#include "buffered-reader.hpp"

#include <cassert>
#include <unistd.h>
#include <cerrno>
#include <sys/types.h>
#include <sys/socket.h>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

using std::span;
using std::byte;

static auto logger = spdlog::stderr_color_mt("net.http.buffered-reader");

namespace net::http
{
    BufferedReader::BufferedReader(int input_fd, span<byte> input_buffer)
        : input_fd(input_fd)
        , input_buffer(input_buffer)
    {
        assert(input_fd != -1);
        assert(input_buffer.size() > 0);
    }

    span<byte> BufferedReader::read_until(span<byte> delimiter, bool skip)
    {
        assert(!delimiter.empty());

        //TODO analyse if it's worth it to pre-call `fetch_more()` method to avoid a dummy first loop trying to
        //     read from an empty read block

        span<byte>::iterator start = read_iterator();
        auto search_cursor = start;
        while (true) {
            // try to find
            auto end = this->input_buffer.begin() + this->fetched_offset;
            auto it = std::search(search_cursor, end, delimiter.begin(), delimiter.end());
            if (it != end) {
                // found it: slice, advance read offset and return
                if (skip) {
                    span<byte> chunk{ start, it };
                    this->read_offset += chunk.size() + delimiter.size();
                    return chunk;
                }
                //!skip -> include the delimiter in the resulting chunk
                span<byte> chunk{ start,  it + delimiter.size() };
                this->read_offset += chunk.size();
                return chunk;
            }

            // not found: fetch more bytes
            // if there is no more free space in the input_buffer to use
            if (is_full()) {
                throw std::runtime_error{"delimiter not found: buffer overflow"};
            }

            auto searched_until = end;
            auto read_chunk = fetch_more();
            if (read_chunk.empty()) {
                // eof... not found!
                throw std::runtime_error{"delimiter not found"};
            }

            search_cursor = searched_until - delimiter.size();
            if (search_cursor < this->input_buffer.begin()) {
                search_cursor = this->input_buffer.begin();
            }
        }

        throw std::logic_error{"unreachable"};
    }

    span<byte> BufferedReader::fetch_more()
    {
        // [ read_offset <= fetched_offset <= input_buffer.size() ]
        assert(this->read_offset <= this->fetched_offset && this->fetched_offset <=  this->input_buffer.size());

        // if there is no more free space in the input_buffer to use
        if (is_full()) {
            throw std::runtime_error{"buffer overflow"};
        }

        size_t chunk_size = free_size();
        byte* chunk_ptr = read_ptr();

        ssize_t nread = ::read(this->input_fd, chunk_ptr, chunk_size);
        this->io_count++;
        // if IO error
        if (nread < 0) {
            int error_code = errno;
            const char* error_reason = strerror(error_code);
            const char* error_msg = "io error: failed to read chunk from socket";
            logger->error("recv() failed: [{}] {}. fd={} size={}",
                error_code, error_reason, this->input_fd, chunk_size);
            throw std::runtime_error{ error_msg };
        }
        // if EOF reached
        if (nread == 0) {
            return {};
        }
        //nread > 0

        span<byte> chunk = this->input_buffer.subspan(this->fetched_offset, nread);

        this->fetched_offset += nread;

        return chunk;
    }

    byte* BufferedReader::read_ptr()
    {
        return this->input_buffer.data() + this->read_offset;
    }

    std::span<std::byte>::iterator BufferedReader::read_iterator()
    {
        return this->input_buffer.begin() + this->read_offset;
    }

    std::byte* BufferedReader::fetched_ptr()
    {
        return this->input_buffer.data() + this->fetched_offset;
    }

    std::span<std::byte>::iterator BufferedReader::fetched_iterator()
    {
        return this->input_buffer.begin() + this->fetched_offset;
    }

    std::size_t BufferedReader::available_size() const
    {
        assert(this->fetched_offset >= this->read_offset);
        return this->fetched_offset - this->read_offset;
    }

    std::size_t BufferedReader::free_size() const
    {
        assert(this->input_buffer.size() >= this->fetched_offset);
        return this->input_buffer.size() - this->fetched_offset;
    }

    bool BufferedReader::is_full() const
    {
        return this->fetched_offset >= this->input_buffer.size();
    }
}
