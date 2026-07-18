// classical c++ standard library headers
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <string_view>
#include <iterator>
#include <algorithm>
#include <iomanip>
#include <span>

// "modern" c++23 standard library headers
//NOTE format and print are not well supported in gcc yet. we're sailing on clangs sea here.
//NOTE gcc14 has implemented most (all?) of what's needed here. let's wait for ubuntu 24 upgrade...
#include <format>
#include <variant>
#include <ranges>
#include <print>

// C standard library headers
#include <cassert>
#include <cstring>

// Posix/Linux/Unix or other OS-specific headers
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

// Third-party libraries
#include <spdlog/spdlog.h>
#include <spdlog/cfg/env.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/mdc.h>

// Our own headers
#include "strutils/strutils.hpp"
#include "net/http/buffered-reader.hpp"

// If you're willing to feel free to factor this out.
// We just don't want to type all of this everytime.
using std::string;
using std::vector;
using std::map;
using std::string_view;
using std::format;

// Handy macros
#define eprint(...)   std::print(stderr, __VA_ARGS__)
#define eprintln(...) std::println(stderr, __VA_ARGS__)

// Global Settings to move later to the Config layer
namespace config {
    static constexpr int ReadTimeoutSeconds  = 3;
    static constexpr int WriteTimeoutSeconds = 3;
}

namespace net
{
    static auto logger = spdlog::stderr_color_mt("net");

    void setsockopt_reuse_or_exit(int socket_fd)
    {
        assert(socket_fd != -1);

        int opt = 1;
        if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) != 0) {
            int error_code = errno;
            const char* error_reason = strerror(error_code);
            net::logger->critical("failed to set tcp/ipv4 socket options: address and port reuse: errno={} reason=\"{}\" socket_fd={}\n",
                error_code, error_reason, socket_fd);
            exit(1);
        }
    }

    void setsockopt_read_timeout_or_throw(int socket_fd, time_t timeout_seconds)
    {
        assert(socket_fd != -1);
        assert(timeout_seconds > 0);

        timeval read_timeout { .tv_sec = timeout_seconds, .tv_usec = 0 };
        if (setsockopt(socket_fd, SOL_SOCKET, SO_RCVTIMEO, &read_timeout, sizeof(read_timeout)) != 0) {
            int error_code = errno;
            const char* error_reason = strerror(error_code);
            logger->error("failed to set tcp/ipv4 socket options: read timeout: errno={} reason=\"{}\" socket_fd={}\n",
                error_code, error_reason, socket_fd);
            throw std::runtime_error{"failed to set socket read timeout"};
        }
    }

    void setsockopt_write_timeout_or_throw(int socket_fd, time_t timeout_seconds)
    {
        assert(socket_fd != -1);
        assert(timeout_seconds > 0);

        timeval write_timeout { .tv_sec = timeout_seconds, .tv_usec = 0 };
        if (setsockopt(socket_fd, SOL_SOCKET, SO_SNDTIMEO, &write_timeout, sizeof(write_timeout)) != 0) {
            int error_code = errno;
            const char* error_reason = strerror(error_code);
            logger->error("failed to set tcp/ipv4 socket options: write timeout: errno={} reason=\"{}\" socket_fd={}\n",
                error_code, error_reason, socket_fd);
            throw std::runtime_error{"failed to set socket write timeout"};
        }
    }

    struct ClientConnection {
        int fd = -1;
        // std::variant<sockaddr_in, sockaddr_in6> addr;
        //TODO add ipv6 support (std::variant<sockaddr_in, sockaddr_in6>?)
        sockaddr_in address;
        //TODO we don't need that to be a field
        socklen_t address_len = sizeof(address);

        ~ClientConnection()
        {
            socket_close();
        }

        void socket_close()
        {
            if (this->fd != -1) {
                eprintln("info: closing client connection...");
                close(this->fd);
                eprintln("info: client connection closed.");
            }
        }

        //NOTE this could be cached or precalculated
        //FIXME how is this wrong??
        [[nodiscard]]
        short get_port() const {
            //return ntohs(this->address.sin_port);
            // return std::get<sockaddr_in>(this->addr).sin_port;
            // return std::get<sockaddr_in6>(this->addr).sin6_port;
            return this->address.sin_port;
        }

        [[nodiscard]]
        const char* get_address() const {
            return inet_ntoa(this->address.sin_addr);
        }
    };

    [[nodiscard]]
    ClientConnection accept_ipv4_or_exit(int server_socket_fd)
    {
        assert(server_socket_fd != -1 && server_socket_fd != 0);

        ClientConnection conn = {
            .address = {},
            .address_len = sizeof(sockaddr_in),
        };

        conn.fd = accept(
            server_socket_fd,
            reinterpret_cast<sockaddr*>(&conn.address),
            &conn.address_len
        );
        if (conn.fd < 0) {
            int error_code = errno;
            const char* error_reason = strerror(error_code);
            eprintln("error: socket accept failed: code={} reason=\"{}\"", error_code, error_reason);
        }
        return conn;
    }
}

namespace net::ip::v4::address
{
    [[nodiscard]]
    sockaddr_in create(const char* host, uint16_t port)
    {
        return {
            .sin_family = AF_INET,
            .sin_port = htons(port),
            .sin_addr = {
                .s_addr = inet_addr(host),
            },
            .sin_zero = {},
        };
    }
}

namespace net::tcp::socket
{
    [[nodiscard]]
    int create_or_exit()
    {
        net::logger->debug("creating tcp/ipv4 socket...");

        int fd = ::socket(AF_INET, SOCK_STREAM, 0);
        if (fd == -1) {
            int error_code = errno;
            const char* error_reason = strerror(error_code);
            net::logger->critical("failed to create tcp/ipv4 socket: code={} reason=\"{}\"", error_code, error_reason);
            exit(1);
        }
        return fd;
    }

    void bind_or_exit(int socket_fd, const char* host, short port)
    {
        assert(socket_fd != -1);
        assert(host != nullptr);
        assert(port > 0);

        net::logger->debug("binding tcp socket to ipv4 address... socket_fd={} address=\"{}:{}\"\n", socket_fd, host, port);

        //NOTE refactor this out if address would be usefull by the caller
        sockaddr_in address = net::ip::v4::address::create(host, port);

        if (bind(socket_fd, reinterpret_cast<struct sockaddr*>(&address), sizeof(address)) != 0) {
            int error_code = errno;
            const char* error_reason = strerror(error_code);
            std::cerr << format(
                "error: tcp/ipv4 socket binding failed: code={} reason=\"{}\" socket_fd={} address=\"{}:{}\"\n",
                error_code, error_reason, socket_fd, host, port
            );
            exit(1);
        }
    }

    void listen_or_exit(int socket_fd, int tcp_backlog)
    {
        assert(socket_fd != -1);
        
        net::logger->debug("socket: listening...");
        if (listen(socket_fd, tcp_backlog) != 0) {
            int error_code = errno;
            const char* error_reason = strerror(error_code);
            std::cerr << format(
                "error: socket listening failed: code={} reason=\"{}\" socket_fd={} tcp_backlog={}\n",
                error_code, error_reason, socket_fd, tcp_backlog
            );
            exit(1);
        }
    }

    struct Socket
    {
        int fd = -1;

        ~Socket()
        {
            if (this->fd != -1) {
                net::logger->debug("closing socket... fd={}", this->fd);
                close(this->fd);
                net::logger->debug("socket closed. fd={}", this->fd);
            }
        }

        void create()
        {
            this->fd = create_or_exit();
            net::setsockopt_reuse_or_exit(this->fd);
        }

        void bind(const char* host, short port)
        {
            bind_or_exit(this->fd, host, port);
        }

        void listen(int tcp_backlog)
        {
            listen_or_exit(this->fd, tcp_backlog);
        }
    };
}

namespace net::http
{
    static constexpr std::array<std::byte, 2> EndOfLine = {
        std::byte{'\r'},
        std::byte{'\n'}
    };

    struct Request {
        // start line
        std::string_view method;
        std::string_view path;
        std::string_view protocol;

        std::map<std::string_view, std::string_view> headers;
    };

    struct Response {
        std::string_view protocol = "HTTP/1.0";
        int status_code = 200;
        std::string_view status_text = "";

        //TODO this should be a writter, but it's ok for now...
        std::string_view body = "";
    };

    struct Context {
        static constexpr int MaxReads = 3;
        
        std::shared_ptr<spdlog::logger> logger = spdlog::stderr_color_mt("httpctx");

        // raw input data until \r\n\r\n is identified.
        // most of references, views and pointers for input data would point to it
        std::array<std::byte, 4096> input_buffer = {};

        const ClientConnection& conn;
        Request request = {};
        Response response = {};

        explicit Context(const ClientConnection& conn) noexcept : conn(conn) {}
        //TODO delete the copy constructor and assignment operator maybe?

        ~Context() {
            spdlog::drop(logger->name());
        }
    };

    void conn_send(const net::ClientConnection& conn, const Response& resp)
    {
        //TODO format allocates. avoid this
        //TODO abstract a way to serialize a response object into data to send into the socket (BufferedWriter?)
        const string resp_data = format(
            "{} {} {}\r\n"
            "Connection: close\r\n"
            "Content-Length: {}\r\n"
            "\r\n"
            "{}",
            resp.protocol,
            resp.status_code,
            resp.status_text,
            resp.body.size(),
            resp.body
        );

        const ssize_t bytes_sent = send(conn.fd, resp_data.data(), resp_data.size(), 0);
        eprintln("info: sent data to client. bytes_count={}", bytes_sent);

        assert((size_t) bytes_sent == resp_data.size());
    }

    void send_bad_request(const net::ClientConnection& conn)
    {
        net::http::Response resp;
        resp.status_code = 400;
        resp.status_text = "Bad Request";
        resp.body = "Bad Request\n";
        conn_send(conn, resp);
    }

    void send_not_found(const net::ClientConnection& conn)
    {
        net::http::Response resp;
        resp.status_code = 404;
        resp.status_text = "Not Found";
        resp.body = "Not Found\n";
        conn_send(conn, resp);
    }

    void send_ok(const net::ClientConnection& conn, string_view resp_body)
    {
        net::http::Response resp;
        resp.status_code = 200;
        resp.status_text = "OK";
        resp.body = resp_body;
        conn_send(conn, resp);
    }
}

namespace net::http::headers
{
    //TODO this could still be an string_view for simplicity
    static constexpr std::array<std::byte, 4> EndOfHeaders = {
        static_cast<std::byte>('\r'),
        static_cast<std::byte>('\n'),
        static_cast<std::byte>('\r'),
        static_cast<std::byte>('\n'),
    };
}

namespace debug
{
    template <typename T>
    void span_eprintln(const char* title, std::span<T> input)
    {
        eprint("=== {} ===\n\"", title);
        for (const T& b: input) {
            switch (b) {
                case std::byte{'\0'}: eprint("\\0"); break;
                case std::byte{'\r'}: eprint("\\r"); break;
                case std::byte{'\n'}: eprint("\\n\n"); break;
                default:
                    std::cerr << static_cast<char>(b);
                    break;
            }
        }
        eprintln("\"\n");
    }
}

namespace server
{
    static void handle_connection(const net::ClientConnection& conn)
    {
        net::http::Context ctx{ conn };
        net::setsockopt_read_timeout_or_throw(conn.fd, config::ReadTimeoutSeconds);
        net::setsockopt_write_timeout_or_throw(conn.fd, config::WriteTimeoutSeconds);

        // reading/parsing loop
        size_t total_bytes_read = 0;
        size_t recv_count = 0;
        while (true) {
            net::logger->debug("starting read loop");
            if (recv_count >= ctx.MaxReads) {
                throw std::runtime_error{"max reads exceeded"};
            };
            errno = 0;
            ssize_t bytes_read = recv(
                conn.fd,
                ctx.input_buffer.data() + total_bytes_read,
                ctx.input_buffer.size() - total_bytes_read,
                0
            );
            int error_code = errno;
            recv_count++;
            if (error_code == EINTR) {
                // EINTR 4 syscall interrupted
                // try again
                continue;
            }
            if (bytes_read < 0) {
                net::logger->warn("recv failed: fd={} errno={} reason=\"{}\"", conn.fd, error_code, strerror(error_code));
                throw std::runtime_error{"io error"};
            }
            if (bytes_read == 0) {
                //check if headers are ok (invalid bytes and "\r\n\r\n")
                break;
            }
            if (total_bytes_read + bytes_read >= ctx.input_buffer.size()) {
                throw std::runtime_error{"request input buffer overflow"};
            }
            total_bytes_read += bytes_read;

            int line_parsing_state = 0;
            for (size_t i = total_bytes_read - bytes_read; i < total_bytes_read - 1; i++) {
                std::byte b = ctx.input_buffer.at(i);
                switch (line_parsing_state) {
                    case 0:
                        if (b == std::byte{'\r'}) {
                            // go to the next state
                            line_parsing_state = 1;
                            continue;
                        }
                        //TODO improve invalid checking
                        if (!isascii(static_cast<int>(b))) {
                            throw std::runtime_error{"bad request: invalid byte"};
                        }
                        // any other valid byte
                        continue;

                    case 1:
                        if (b == std::byte{'\n'}) {
                            //got a new line!

                        }
                    default:
                        break;
                }
                // std::byte b1 = ctx.input_buffer.at(i+1);
            }
        }
    }

    static void handle_connection_old_2(const net::ClientConnection& conn)
    {
        //TODO improve this
        static int rid = 0;
        spdlog::mdc::put("id", std::format("{}", ++rid));
        net::http::Context ctx{ conn };

        net::setsockopt_read_timeout_or_throw(conn.fd, config::ReadTimeoutSeconds);
        net::setsockopt_write_timeout_or_throw(conn.fd, config::WriteTimeoutSeconds);

        //TODO maybe evolve this reading loop to chunk lines to parser
        // reading loop + basic headers validation
        size_t total_bytes_read = 0;
        size_t recv_count = 0;
        std::byte* headers_end_it;
        while (1) {
            net::logger->debug("starting read loop");
            if (recv_count >= ctx.MaxReads) {
                throw std::runtime_error{"max reads exceeded"};
            };
            errno = 0;
            ssize_t bytes_read = recv(
                conn.fd,
                ctx.input_buffer.data() + total_bytes_read,
                ctx.input_buffer.size() - total_bytes_read,
                0
            );
            int error_code = errno;
            recv_count++;
            if (error_code == EINTR) {
                // EINTR 4 syscall interrupted
                // try again
                continue;
            }
            if (bytes_read < 0) {
                net::logger->warn("recv failed: fd={} errno={} reason=\"{}\"", conn.fd, error_code, strerror(error_code));
                throw std::runtime_error{"io error"};
            }
            if (bytes_read == 0) {
                //check if headers are ok (invalid bytes and "\r\n\r\n")
                break;
            }
            if (total_bytes_read + bytes_read >= ctx.input_buffer.size()) {
                throw std::runtime_error{"request input buffer overflow"};
            }
            total_bytes_read += bytes_read;

            //TODO search for invalid bytes...
            // search for '\r\n\r\n' or invalid bytes...
            headers_end_it = std::search(
                ctx.input_buffer.begin() + total_bytes_read - bytes_read, ctx.input_buffer.begin() + total_bytes_read,
                net::http::headers::EndOfHeaders.cbegin(), net::http::headers::EndOfHeaders.cend()
            );
            if (headers_end_it == ctx.input_buffer.end()) {
                eprintln("warn: io: end of headers not found. recv_count={}", recv_count);
                continue;
            }

            break;
        }

        std::span<std::byte> headers{
            ctx.input_buffer.begin(),
            headers_end_it + 2  // include "\r\n" into the headers span
        };
        
        debug::span_eprintln("headers", headers);

        std::string_view headers_sv = strutils::to_string_view(headers);
        net::logger->info(headers_sv);

        // std::stringstream headers_line_stream{  };

        //TODO parse headers to map

        string_view resp_body =
            "<!DOCTYPE html>\n"
            "<html lang=\"en\">\n"
            "<head>\n"
            "    <meta charset=\"UTF-8\">\n"
            "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
            "    <title>HTTP Server with C++23</title>\n"
            "</head>\n"
            "<body>\n"
            "    <h1>It Works!</h1>\n"
            "</body>\n"
            "</html>\n";
        net::http::send_ok(conn, resp_body);
    }

    [[deprecated]]
    static void handle_connection_old(const net::ClientConnection& conn)
    {
        // # Design Notes
        //
        // - Headers are case insensitive. Consider normalization (at least when getting it)
        // - Header Names cannot have spaces, but Header Values can. Trailling space in Header Values should be dropped
        // - Status Line and Headers should only contain printable ascii and whitespaces (at least \r\n\t and ' ')
        //     Although care should be taken for cookie values (check what is actually valid or not)
        // - A browser should be able to accept at least 300 cookies with a maximum size of 4096 bytes, as stipulated by RFC (total 1228800 bytes)
        // - Headers are unordered, so Host and Content-Lenght could be at the bottom
        // - Unbounded reading/buffering (until \r\n\r\n) is not good
        // - Reads (recv) can give you less bytes than the requested (then what to do?)
        // - Request lines should also be bounded by a limit
        // - Timeout's (and other limites) would also be desired
        // - Maybe we can sanitize and find for \r\n\r\n in a single pass

        //TODO review this object and add more stuff in it
        net::http::Request request;

        //TODO maybe move this into the request object so string_views and span's are all safely encapsulated inside it
        //TODO change this back to vector (from request::raw_buffer) and read multiple times until \r\n\r\n is found
        std::array<std::byte, 8 * 1024> read_buffer = {};

        //TODO refactor this into a function that returns the span from the buffer
        ssize_t bytes_received = recv(conn.fd, read_buffer.data(), read_buffer.size(), 0);
        if (bytes_received < 0) {
            int error_code = errno;
            const char* error_reason = strerror(error_code);
            eprintln("warn: failed to receive bytes from client: code={} reason=\"{}\"", error_code, error_reason);
            net::http::send_bad_request(conn);
            return;
        }
        if (bytes_received == 0) {
            eprintln("warn: unexpected EOF for the first read");
            return;
        }
        eprintln("info: total bytes received: {}", bytes_received);

        //NOTE static_cast here is ok because we've checked above that it must not be negative
        std::span<std::byte> input{ read_buffer.begin(), static_cast<size_t>(bytes_received) };
        debug::span_eprintln("input", input);

        //NOTE until sanitization, keep in mind that read_buffer is unsafe
        auto headers_end_it = std::search(
            input.begin(), input.end(),
            net::http::headers::EndOfHeaders.cbegin(), net::http::headers::EndOfHeaders.cend()
        );
        if (headers_end_it == input.end()) {
            eprintln("warn: bad request: end of headers not found");
            net::http::send_bad_request(conn);
            return;
        }
        std::span<std::byte> headers{ input.begin(), headers_end_it + 2 };
        //TODO feed this into the buffer of the BodyReader, because it may contain body data yet to be parsed
        std::span<std::byte> body_part{
            headers_end_it + net::http::headers::EndOfHeaders.size(),
            input.end()
        };
        debug::span_eprintln("headers", headers);

        bool first_line = true;
        auto hs = headers;
        for (
            auto header_it = std::search(
                hs.begin(), hs.end(),
                net::http::EndOfLine.cbegin(), net::http::EndOfLine.cend()
            );
            header_it != hs.end();
            hs = std::span<std::byte>{
                header_it + net::http::EndOfLine.size(),
                headers.end()
            },
            header_it = std::search(
                hs.begin(), hs.end(),
                net::http::EndOfLine.cbegin(), net::http::EndOfLine.cend()
            )
        ) {
            std::span<std::byte> line{ hs.begin(), header_it };
            debug::span_eprintln("line", line);
            if (first_line) {
                first_line = false;

                auto end_of_method = std::find(line.begin(), line.end(), std::byte{' '});
                if (end_of_method == line.end()) {
                    eprintln("warn: bad request: bad status line");
                    net::http::send_bad_request(conn);
                    return;
                }
                std::span<std::byte> rest{ end_of_method + 1, line.end() };
                auto end_of_path = std::find(rest.begin(), rest.end(), std::byte{' '});
                if (end_of_path == rest.end()) {
                    eprintln("warn: bad request: bad status line");
                    net::http::send_bad_request(conn);
                    return;
                }
                std::span<std::byte> method{ line.begin(), end_of_method };
                std::span<std::byte> path{ rest.begin(), end_of_path };
                std::span<std::byte> protocol{ end_of_path + 1, line.end() };
                debug::span_eprintln("method", method);
                debug::span_eprintln("path", path);
                debug::span_eprintln("protocol", protocol);

                //TODO now we can validate for non printable bytes...

                request.method = strutils::to_string_view(method);
                request.path = strutils::to_string_view(path);
                request.protocol = strutils::to_string_view(protocol);

            } else {
                static constexpr std::array<std::byte, 2> delim = { std::byte{':'}, std::byte{' '} };
                auto it = std::search(line.begin(), line.end(), delim.begin(), delim.end());
                if (it == line.end()) {
                    eprintln("warn: bad request: bad header");
                    net::http::send_bad_request(conn);
                    return;
                }
                std::span<std::byte> header_key{ line.begin(), it };
                std::span<std::byte> header_val{ it + delim.size(), line.end() };
                debug::span_eprintln("header key", header_key);
                debug::span_eprintln("header value", header_val);

                //TODO now we can validate for non printable bytes...

                request.headers[strutils::to_string_view(header_key)] = strutils::to_string_view(header_val);
            }
        }

        eprintln("info: finished parsing headers");

        // Routing

        if (request.method.compare("GET") == 0 && request.path.compare("/") == 0) {
           string_view resp_body =
                "<!DOCTYPE html>\n"
                "<html lang=\"en\">\n"
                "<head>\n"
                "    <meta charset=\"UTF-8\">\n"
                "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
                "    <title>HTTP Server with C++23</title>\n"
                "</head>\n"
                "<body>\n"
                "    <h1>It Works!</h1>\n"
                "</body>\n"
                "</html>\n";
            net::http::send_ok(conn, resp_body);
            return;
        }
        
        net::http::send_not_found(conn);
    }
}

int main(void)
{
    spdlog::cfg::load_env_levels("LOG_LEVEL");
    spdlog::set_pattern("[%Y-%m-%dT%H:%M:%S.%e%z] [pid:%P tid:%t] [%n] %^%l%$: %v %&");

    auto logger = spdlog::stderr_color_st("main");

    std::array<std::byte, 24> buffer = {};
    net::http::BufferedReader buffered_reader{
        STDIN_FILENO,
        { buffer.begin(), buffer.size() }
    };
    std::byte delimiter[] = {
        std::byte{','},
        // std::byte{'d'},
        // std::byte{'e'},
        // std::byte{'f'},
    };
    std::span<std::byte> delimiter_span{ delimiter };

    auto chunk1 = buffered_reader.read_until(delimiter_span);
    std::printf("[%zu] %.*s\n", chunk1.size(), (int) chunk1.size(), reinterpret_cast<const char*>(chunk1.data()));

    chunk1 = buffered_reader.read_until(delimiter_span);
    std::printf("[%zu] %.*s\n", chunk1.size(), (int) chunk1.size(), reinterpret_cast<const char*>(chunk1.data()));

    exit(0);

    (void) server::handle_connection_old;
    (void) server::handle_connection_old_2;

    constexpr const char* host = "127.0.0.1";
    constexpr short port = 8080;
    constexpr int tcp_backlog = 32;

    logger->info("server is initializing...");

    net::tcp::socket::Socket server_socket;
    server_socket.create();
    server_socket.bind(host, port);
    server_socket.listen(tcp_backlog);

    logger->info("server is ready. url=\"http://{}:{}\"", host, port);

    // eprintln("info: server is ready. url=\"http://{}:{}\"", host, port);

    while (true) {
        //NOTE ClientConnection is a RAII over the client socket fd.
        //     After its lifecycle, it'll close it.
        net::ClientConnection conn = net::accept_ipv4_or_exit(server_socket.fd);
        logger->info("new client connected. address=\"{}\" port={}", conn.get_address(), conn.get_port());

        try {
            // server::handle_connection(conn);
            server::handle_connection(conn);
            logger->info("finished handling client connection");
        } catch (const std::exception& e) {
            logger->info("{}", e.what());
        }
    }
}
