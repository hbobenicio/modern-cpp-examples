#include "Parser.h"
#include <stack>
#include <queue>
#include <cctype>
#include <iostream>
#include <stdexcept>
#include <unordered_map>

namespace calculator::core::parser {

    inline static bool is_operator(char c) {
        return (c == '+' || c == '-' || c == '*' || c == '/');
    }

    std::queue<char> shunting_yard(const std::string& input) {
        static const std::unordered_map<char, short> precedence {
            { '+', 2 },
            { '-', 2 },
            { '*', 3 },
            { '/', 3 },
        };

        std::stack<char> operators;
        std::queue<char> output;

        for (const char& c: input) {
            if (isspace(c)) {
                continue;
            }

            // TODO improve this to support full numbers, not just digits
            if (isdigit(c)) {
                output.push(c);
                continue;
            }

            if (is_operator(c)) {
                if (!operators.empty()) {
                    char top = operators.top();
                    while (precedence.at(top) >= precedence.at(c) && !operators.empty()) {
                        top = operators.top();
                        output.push(top);
                        operators.pop();
                    }
                }
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
        std::stack<char> result;

        while(!input.empty()) {
            const char& c = input.front();
            input.pop();

            if (isdigit(c)) {
                result.push(c);
                continue;
            }

            if (is_operator(c)) {
                const char& op1 = result.top();
                int x = op1 - '0';
                result.pop();

                const char& op2 = result.top();
                int y = op2 - '0';
                result.pop();

                switch (c) {
                    case '+': {
                        int res = x + y;
                        result.push(res + '0');
                        break;

                    }
                    case '-': {
                        int res = x - y;
                        result.push(res + '0');
                        break;
                    }
                    case '*': {
                        int res = x * y;
                        result.push(res + '0');
                        break;
                    }
                    case '/': {
                        int res = x / y;
                        result.push(res + '0');
                        break;
                    }
                    default:
                        throw std::logic_error{ "rpn expression has an unsupported operator" };
                        break;
                }
            }
        }

        if (result.size() != 1) {
            throw std::logic_error{ "rpn stack has not only one value after evaluation" };
        }

        std::cout << result.top() << '\n';
    }

}
