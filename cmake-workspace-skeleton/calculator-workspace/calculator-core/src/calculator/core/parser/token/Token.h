#ifndef CALCULATOR_CORE_PARSER_TOKEN_TOKEN_H
#define CALCULATOR_CORE_PARSER_TOKEN_TOKEN_H

#include <string>
#include <deque>
#include <memory>

namespace calculator::core::parser::token {

    // TODO Create an enum to simplify comparisons
    constexpr static const char* NUM = "NUM";
    constexpr static const char* OPERATOR = "OPERATOR";

    struct Token {
        virtual const char* type() const noexcept = 0;
    };

    using Deque = std::deque<std::unique_ptr<Token>>;

    struct Num: public Token {
        double value = 0.0;

        Num(double value) noexcept;
        const char* type() const noexcept override;
    };

    struct Operator: public Token {
        char value = '\0';

        Operator(char value) noexcept;
        const char* type() const noexcept override;
    };

}

#endif
