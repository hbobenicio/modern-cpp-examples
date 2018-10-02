#include <calculator/core/Calculator.h>

namespace calculator {
namespace core {

void Calculator::add(double x) noexcept {
  _total += x;
}

void Calculator::sub(double x) noexcept {
  _total -= x;
}

void Calculator::mult(double x) noexcept {
  _total *= x;
}

void Calculator::div(double x) {
  // TODO throw when x is close enough to zero
  _total /= x;
}

double Calculator::getTotal() const noexcept {
  return _total;
}

}
}
