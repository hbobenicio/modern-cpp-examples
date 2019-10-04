#include "Lexer.h"

#include <sstream>

namespace calculator::core::parser {

    token::Deque lexer(const std::string& input) {
        token::Deque output;

        for (auto it = std::begin(input); it != std::end(input); it++) {
            char c = *it;

            if (isdigit(c)) {
                std::stringstream num;
                num << c;

                auto next = std::next(it);
                while (next != std::end(input) && isdigit(*next)) {
                    num << *next;
                    next = next + 1;
                    it++;
                }

                double val = 0.0;
                num >> val;

                output.push_back(std::move(std::make_unique<token::Num>(val)));

            } else if (c == '+' || c == '-' || c == '*' || c == '/') {
                output.push_back(std::move(std::make_unique<token::Operator>(c)));
            }
        }

        return output;
    }

}
