#include <iostream>
#include <calculator/core/Calculator.h>

int main(int argc, char* argv[]) {
    // TODO convert this input from argument to stdin
    if (argc != 2) {
        std::cerr << "error: missing argument: please, provide a simple mathematic expression as the first argument for this program\n";
        return EXIT_FAILURE;
    }

    calculator::core::Calculator calc;
    calc.parse(argv[1]);
}
