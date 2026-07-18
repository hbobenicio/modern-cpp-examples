#pragma once

#include <string_view>
#include <ctype.h>

namespace strutils
{
    static constexpr std::string_view Spaces = " \f\n\r\t\v";

    bool is_blank(std::string_view s);
    inline bool is_not_blank(std::string_view s);

    std::string_view trim_left(std::string_view sv);
    std::string_view trim_right(std::string_view sv);
    std::string_view trim(std::string_view sv);

    template <typename T>
    std::string_view to_string_view(const T& xs);
}
