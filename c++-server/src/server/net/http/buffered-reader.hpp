#pragma once

#include <span>
#include <cstddef>

namespace net::http
{
    class BufferedReader {
    private:
        /**
         * The input File Descriptor where read/recv syscalls will refer to
         */
        int input_fd;

        /**
         * The input buffer where we'd store read data into
         */
        std::span<std::byte> input_buffer;

        /**
         * The read offset relative to input_buffer that indicates until which byte offset did we already read
         */
        // size_t read_offset = 0;
        std::size_t read_offset = 0;

        /**
         * The total amount of bytes we've already consumed of the input_buffer
         */
        std::size_t fetched_offset = 0;

        /**
         * The count of I/O read operations performed
         */
        std::size_t io_count = 0;

    public:
        /**
         * The main constructor.
         * 
         * @param input_fd     The input file descriptor where to read/recv from
         * @param input_buffer The input buffer where bytes would be read into
         */
        BufferedReader(int input_fd, std::span<std::byte> input_buffer);

        /**
         * Reads until we find a sequence of bytes.
         * 
         * The delimiter must be found or else an exception is thrown.
         * 
         * @param delimiter A byte span that indicates the stop condition of the read operation
         * @param skip include or not the delimiter in the result span
         * @throws std::runtime_error in case of errors
         */
        std::span<std::byte> read_until(std::span<std::byte> delimiter, bool skip = true);

    private:
        std::span<std::byte> fetch_more();

        [[nodiscard]]
        std::byte* read_ptr();

        [[nodiscard]]
        std::span<std::byte>::iterator read_iterator();

        [[nodiscard]]
        std::byte* fetched_ptr();

        [[nodiscard]]
        std::span<std::byte>::iterator fetched_iterator();
        
        [[nodiscard]]
        std::size_t available_size() const;

        [[nodiscard]]
        std::size_t free_size() const;

        [[nodiscard]]
        bool is_full() const;
    };
}
