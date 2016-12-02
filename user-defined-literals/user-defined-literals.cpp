#include <iostream>
#include <sstream>
#include <cstdio>
#include <iomanip>
#include <string>

/**
 * Per paragraph 13.5.8./3 of the C++11 Standard on user-defined literals:
 *
 *     The declaration of a literal operator shall have a parameter-declaration-clause equivalent to one of the following:
 *
 *     const char*
 *     unsigned long long int
 *     long double
 *     char
 *     wchar_t
 *     char16_t
 *     char32_t
 *     const char*, std::size_t
 *     const wchar_t*, std::size_t
 *     const char16_t*, std::size_t
 *     const char32_t*, std::size_t
 */

//using namespace std;
using time_unit = size_t;

class Time
{
protected:
	time_unit t;

public:
	Time(ullint h, ullint min, ullint s)
		: h(h), min(min), s(s) {}

	ullint hour() const {
		return h;
	}

	ullint minutes() const {
		return min;
	}

	ullint seconds() const {
		return s;
	}

	std::string str() const {
		std::ostringstream ss;
		ss << std::setw(2) << std::setfill('0') << h;
		ss << ':';
		ss << std::setw(2) << std::setfill('0') << min;
		ss << ':';
		ss << std::setw(2) << std::setfill('0') << s;
		return ss.str();
	}
};

Time operator ""_hour(ullint h) {
	return Time{h, 0, 0};
}

int main()
{
	using namespace std;

	auto time = 1_hour;
	cout << time.str() << endl;
}

