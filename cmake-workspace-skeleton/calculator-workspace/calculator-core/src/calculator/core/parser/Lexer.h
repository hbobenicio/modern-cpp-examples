#ifndef CALCULATOR_CORE_PARSER_LEXER_H
#define CALCULATOR_CORE_PARSER_LEXER_H

#include "token/Token.h"

namespace calculator::core::parser {

    token::Deque lexer(const std::string& input);

}

#endif
