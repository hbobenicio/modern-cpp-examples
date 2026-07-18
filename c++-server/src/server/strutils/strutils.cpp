#include "strutils.hpp"

namespace strutils
{
    using std::string_view;
    
    bool is_blank(string_view s)
    {
        for (auto c: s) {
            if (!isspace(c)) {
                return false;
            }
        }
        return true;
    }

    inline bool is_not_blank(string_view s)
    {
        return !is_blank(s);
    }

    // static constexpr string_view Spaces = " \f\n\r\t\v";

    string_view trim_left(string_view sv)
    {
        size_t first_non_space = sv.find_first_not_of(Spaces);
        size_t remove_count = std::distance(sv.cbegin(), sv.cbegin() + first_non_space);
        sv.remove_prefix(remove_count);
        return sv;
    }

    string_view trim_right(string_view sv)
    {
        size_t last_non_space = sv.find_last_not_of(Spaces);
        size_t remove_count = std::distance(sv.cbegin() + last_non_space, sv.cend()) - 1;
        //NOTE I think que could use remove_suffix and simplify the expression above
        sv.remove_prefix(remove_count);
        return sv;
    }

    string_view trim(string_view sv)
    {
        return trim_right(trim_left(sv));
    }

    template <typename T>
    string_view to_string_view(const T& xs)
    {
        return {
            reinterpret_cast<const char*>(xs.data()),
            xs.size() * sizeof(*xs.data())
        };
    }
}
