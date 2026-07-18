#pragma once

namespace os::posix
{
    struct PosixError {
        int code;
        const char* reason;
        const char* msg;

        PosixError(const char* msg = nullptr);
    };
}
