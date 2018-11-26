/**
 * C++17 Structured Biding Example.
 *
 * Compile with something like this:
 *
 * $ clang++-7 -std=c++17 -Wall -pedantic -Werror main.cpp
 */
#include <iostream>
#include <tuple>
#include <cmath>
#include <limits>

static constexpr float EPSILON = 0.00001f;
static constexpr float MONEY_EPSILON = 0.01f;

using DivideResult = std::tuple<float, const char*>;

static DivideResult divide(float x, float y, float eps = EPSILON) {
	if (fabs(y) < eps) {
		return DivideResult{0.0f, "cannot divide by zero!"};
	}
	return DivideResult{x / y, nullptr};
}

int main() {
	const auto [x, err] = divide(1.0f, 0.009f, MONEY_EPSILON);
	if (err) {
		std::cerr << "division error: " << err << '\n';
		exit(EXIT_FAILURE);
	}

	std::cout << x << '\n';
}

