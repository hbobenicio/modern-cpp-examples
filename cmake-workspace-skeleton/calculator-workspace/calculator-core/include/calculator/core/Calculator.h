#ifndef CALCULATOR_H
#define CALCULATOR_H

namespace calculator {
namespace core {

class Calculator {
private:
  double _total = 0.0;

public:
  void add(double x) noexcept;
  void sub(double x) noexcept;
  void mult(double x) noexcept;
  void div(double x);

  double getTotal() const noexcept;
};

} // core
} // calculator

#endif
