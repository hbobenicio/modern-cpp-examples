#include <calculator/core/Calculator.h>
#include <iostream>

#include "parser/Parser.h"

namespace calculator::core {

    void Calculator::parse(const std::string& expression) {
        std::string infix_expression {expression};
        std::queue<char> rpn = parser::shunting_yard(infix_expression);
        // while (!rpn.empty()) {
        //     const char& c = rpn.front();
        //     std::cout << c;
        //     rpn.pop();
        // }
        // std::cout << '\n';
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
