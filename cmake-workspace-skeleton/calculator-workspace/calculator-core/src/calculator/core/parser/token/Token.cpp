#include "Token.h"

namespace calculator::core::parser::token {

    Num::Num(double value) noexcept
        : value(value) {
    }

    const char* Num::type() const noexcept {
        return NUM;
    }

    Operator::Operator(char value) noexcept
        : value(value) {
    }

    const char* Operator::type() const noexcept {
        return OPERATOR;
    }

}
