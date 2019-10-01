#ifndef CALCULATOR_CORE_CALCULATOR_H
#define CALCULATOR_CORE_CALCULATOR_H

#include <string>

namespace calculator::core {

    class Calculator {
    protected:
        double total = 0.0;

    public:
        void parse(const std::string& expression);

        double getTotal() const noexcept;
    };

}

#endif
