#include <calculator/core/Calculator.h>
#include <iostream>

#include "parser/Lexer.h"
#include "parser/Parser.h"

namespace calculator::core {

    void Calculator::parse(const std::string& input) {

        // Lexical Analysis (Tokenization of the input)
        parser::token::Deque tokens = parser::lexer(input);

        parser::token::Deque rpn = parser::shunting_yard(tokens);

        this->total = parser::rpn_eval(rpn);

        // TODO add the response to the total value
    }

    double Calculator::getTotal() const noexcept {
        return this->total;
    }

}
