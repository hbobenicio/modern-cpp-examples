#include <calculator/core/Calculator.h>
#include <iostream>

#include "parser/Parser.h"

namespace calculator::core {

    void Calculator::parse(const std::string& expression) {
        std::queue<char> rpn = parser::shunting_yard(expression);
        parser::rpn_eval(rpn);

        // TODO add the response to the total value
    }

    double Calculator::getTotal() const noexcept {
        return this->total;
    }

}
