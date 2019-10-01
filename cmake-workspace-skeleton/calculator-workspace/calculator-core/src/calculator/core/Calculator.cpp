#include <calculator/core/Calculator.h>
#include <iostream>

#include "parser/Parser.h"

namespace calculator::core {

    void Calculator::parse(const std::string& expression) {
        std::queue<char> rpn = parser::shunting_yard(expression);
        parser::rpn_eval(rpn);
    }

    void Calculator::add(double x) noexcept {
        this->total += x;
    }

    void Calculator::sub(double x) noexcept {
        this->total -= x;
    }

    void Calculator::mult(double x) noexcept {
        this->total *= x;
    }

    void Calculator::div(double x) {
        // TODO throw when x is close enough to zero
        this->total /= x;
    }

    double Calculator::getTotal() const noexcept {
        return this->total;
    }

}
