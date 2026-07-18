#include "posix.hpp"

#include <iostream>
#include <cstring>
#include <cerrno>

#include <format>

namespace os::posix
{
    PosixError::PosixError(const char* msg)
        : code(errno), msg(msg)
    {
        this->reason = ::strerror(this->code);
    }
}
