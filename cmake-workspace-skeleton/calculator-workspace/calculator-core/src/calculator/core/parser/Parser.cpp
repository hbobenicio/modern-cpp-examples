#include "Parser.h"
#include <stack>
#include <queue>
#include <cctype>
#include <iostream>
#include <stdexcept>

namespace calculator {
namespace core {
namespace parser {

std::queue<char> shunting_yard(const std::string& input) {
    std::stack<char> operators;
    std::queue<char> output;

    for (const char& c: input) {
        if (isspace(c)) {
            continue;
        }

        // TODO improve this to support numbers, not just digits
        if (isdigit(c)) {
            output.push(c);
            continue;
        }

        // TODO improve this to support operators precedence
        if (c == '+' || c == '-') {
            operators.push(c);
            continue;
        }
    }

    while (!operators.empty()) {
        const char& c = operators.top();
        output.push(c);
        operators.pop();
    }

    // TODO Validate if expression is balanced (i.e. the number of operators is equal to the number of numbers + 1)

    return output;
}

void rpn_eval(std::queue<char>& input) {
    std::stack<char> calc;

    while(!input.empty()) {
        const char& c = input.front();
        input.pop();

        if (isdigit(c)) {
            calc.push(c);
            continue;
        }

        if (c == '+' || c == '-') {
            const char& op1 = calc.top();
            calc.pop();

            const char& op2 = calc.top();
            calc.pop();

            if (c == '+') {
                int x = op1 - '0';
                int y = op2 - '0';
                int res = x + y;
                calc.push(res + '0');
                continue;
            }

            if (c == '-') {
                int x = op1 - '0';
                int y = op2 - '0';
                int res = x - y;
                calc.push(res + '0');
            }
        }
    }

    if (calc.size() != 1) {
        throw std::logic_error{"rpn stack has not only one value after evaluation"};
    }

    std::cout << calc.top() << '\n';
}

} // parser
} // core
} // calculator
