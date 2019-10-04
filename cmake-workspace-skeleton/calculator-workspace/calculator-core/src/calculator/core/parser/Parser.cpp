#include "Parser.h"
#include <cstring>
#include <stack>
#include <queue>
#include <cctype>
#include <iostream>
#include <stdexcept>
#include <unordered_map>

namespace calculator::core::parser {

    inline static bool is_operator(char c) noexcept {
        return (c == '+' || c == '-' || c == '*' || c == '/');
    }

    token::Deque queue_copy(const token::Deque& input) {
        token::Deque output;

        for (const auto& x: input) {

            if (strcmp(x->type(), token::NUM) == 0) {
                token::Num* num = dynamic_cast<token::Num*>(x.get());
                output.push_back(std::make_unique<token::Num>(num->value));

            } else if (strcmp(x->type(), token::OPERATOR) == 0) {
                token::Operator* op = dynamic_cast<token::Operator*>(x.get());
                output.push_back(std::make_unique<token::Operator>(op->value));

            } else {
                std::cerr << "unsupported token: " << x->type() << '\n';
                throw std::logic_error { "unsupported token" };
            }
        }

        return output;
    }

    token::Deque shunting_yard(const token::Deque& input_expression) {
        static const std::unordered_map<char, short> precedence {
            { '+', 2 },
            { '-', 2 },
            { '*', 3 },
            { '/', 3 },
        };

        token::Deque input = queue_copy(input_expression);

        std::stack<char> operators;
        token::Deque output;

        while (!input.empty()) {
            auto& token = input.front();
            // std::cerr << "DBG> " << token->type() << '\n';

            if (strcmp(token->type(), token::NUM) == 0) {
                token::Num* num = dynamic_cast<token::Num*>(token.get());
                if (num) {
                    token.release();
                    output.emplace_back(num);
                }
            } else if (strcmp(token->type(), token::OPERATOR) == 0) {
                token::Operator* op = dynamic_cast<token::Operator*>(token.get());
                if (op) {
                    token.release();
                    if (!operators.empty()) {
                        char top = operators.top();
                        while (precedence.at(top) >= precedence.at(op->value) && !operators.empty()) {
                            top = operators.top();
                            output.push_back(std::make_unique<token::Operator>(top));
                            operators.pop();
                        }
                    }
                    operators.push(op->value);
                }
            } else {
                std::cerr << "unsupported token: " << token->type() << '\n';
                throw std::logic_error { "unsupported token" };
            }

            input.pop_front();
        }

        while (!operators.empty()) {
            const char& c = operators.top();
            output.push_back(std::make_unique<token::Operator>(c));
            operators.pop();
        }

        // TODO Validate if expression is balanced (i.e. the number of operators is equal to the number of numbers + 1)

        return output;
    }

    void rpn_eval(const token::Deque& input_expression) noexcept(false) {

        token::Deque input = queue_copy(input_expression);
        std::stack<double> result;

        while(!input.empty()) {
            auto& token = input.front();

            if (strcmp(token->type(), token::NUM) == 0) {
                token::Num* num = dynamic_cast<token::Num*>(token.get());
                if (num) {
                    token.release();
                    result.push(num->value);
                }
            } else if (strcmp(token->type(), token::OPERATOR) == 0) {
                token::Operator* op = dynamic_cast<token::Operator*>(token.get());
                if (op) {
                    double y = result.top();
                    result.pop();

                    double x = result.top();
                    result.pop();

                    switch (op->value) {
                        case '+': {
                            double res = x + y;
                            result.push(res);
                            break;

                        }
                        case '-': {
                            double res = x - y;
                            result.push(res);
                            break;
                        }
                        case '*': {
                            double res = x * y;
                            result.push(res);
                            break;
                        }
                        case '/': {
                            double res = x / y;
                            result.push(res);
                            break;
                        }
                        default:
                            throw std::logic_error{ "rpn expression has an unsupported operator" };
                            break;
                    }
                }
            } else {
                std::cerr << "unsupported token: " << token->type() << '\n';
                throw std::logic_error { "unsupported token" };
            }

            input.pop_front();
        }

        if (result.size() != 1) {
            throw std::logic_error{ "rpn stack has not only one value after evaluation" };
        }

        std::cout << result.top() << '\n';
    }

}
