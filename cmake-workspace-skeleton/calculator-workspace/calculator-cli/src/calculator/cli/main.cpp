#include <iostream>
#include <calculator/core/Calculator.h>

int main(int argc, char* argv[]) {
    calculator::core::Calculator calc;

    calc.add(10.0); // total ~= 10.0
    calc.sub(5.0); // total ~= 5.0
    calc.mult(3.0); // total ~= 15.0
    calc.div(10.0); // total ~= 1.5

    // Expected output: "Total: 1.5"
    std::cout << "Total: " << calc.getTotal() << '\n';
}
