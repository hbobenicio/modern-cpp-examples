#include "Lexer.h"

namespace calculator::core::parser {

    token::Deque lexer(const std::string& input) {
        token::Deque output;

        for (char c: input) {
            if (isdigit(c)) {
                double val = atof(&c);
                output.push_back(std::move(std::make_unique<token::Num>(val)));
            } else if (c == '+' || c == '-' || c == '*' || c == '/') {
                output.push_back(std::move(std::make_unique<token::Operator>(c)));
            }
        }

        return output;
    }

}
