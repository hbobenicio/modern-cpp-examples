#ifndef CALCULATOR_CORE_SHUNTING_YARD_H
#define CALCULATOR_CORE_SHUNTING_YARD_H

#include <string>
#include <queue>
#include "Lexer.h"

namespace calculator::core::parser {

    token::Deque shunting_yard(const token::Deque& input);

    void rpn_eval(const token::Deque& input) noexcept(false);
}

#endif
