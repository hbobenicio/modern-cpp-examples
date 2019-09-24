#include <calculator/core/Calculator.h>

namespace calculator {
namespace core {

void Calculator::add(double x) noexcept {
    this->total += x;
}

void Calculator::sub(double x) noexcept {
    this->total -= x;
}

void Calculator::mult(double x) noexcept {
    this->total *= x;
}

void Calculator::div(double x) {
    // TODO throw when x is close enough to zero
    this->total /= x;
}

double Calculator::getTotal() const noexcept {
    return this->total;
}

}
}
