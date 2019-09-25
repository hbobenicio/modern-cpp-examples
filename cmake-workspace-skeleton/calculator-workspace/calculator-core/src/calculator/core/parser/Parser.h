#ifndef CALCULATOR_CORE_SHUNTING_YARD_H
#define CALCULATOR_CORE_SHUNTING_YARD_H

#include <string>
#include <queue>

namespace calculator {
namespace core {
namespace parser {

std::queue<char> shunting_yard(const std::string& input);
void rpn_eval(std::queue<char>& input);

} // parser
} // core
} // calculator

#endif
