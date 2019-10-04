#include <iostream>
#include <string>
#include <calculator/core/Calculator.h>

int main(int argc, char* argv[]) {
    calculator::core::Calculator calc;

    std::string input;
    while (std::getline(std::cin, input)) {
        if (input.length() == 0) {
            std::cerr << "[WARN] ignorando entrada vazia\n";
            continue;
        }

        calc.parse(input);
        std::cout << calc.getTotal() << '\n';
    }
}
