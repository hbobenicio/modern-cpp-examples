#include <iostream>
#include <string>
#include <calculator/core/Calculator.h>

int main(int argc, char* argv[]) {
    calculator::core::Calculator calc;

    std::string input;
    while (std::getline(std::cin, input)) {
        calc.parse(input);
    }
}
