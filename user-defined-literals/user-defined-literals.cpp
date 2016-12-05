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
using ullint = unsigned long long int;

class Time
{
protected:
	ullint h, min, s;

public:
	Time(ullint h, ullint min, ullint s)
		: h(h), min(min), s(s) {}

	ullint hour() const noexcept { return h; }
	ullint minutes() const noexcept { return min; }
	ullint seconds() const noexcept { return s; }

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

Time operator ""_min(ullint m) {
	return Time{0, m, 0};
}

Time operator ""_s(ullint s) {
	return Time{0, 0, s};
}

Time operator ""_timestamp(const char* format, std::size_t size) {
	char aux[] = { format[0], format[1] };
	ullint h = atoi(aux);

	aux[0] = format[3];
	aux[1] = format[4];
	ullint m = atoi(aux);

	aux[0] = format[6];
	aux[1] = format[7];
	ullint s = atoi(aux);

	return Time{h, m, s};
}

int main()
{
	using namespace std;

	auto t1 = 1_hour;
	cout << t1.str() << '\n';

	auto t2 = 30_min;
	cout << t2.str() << '\n';

	auto t3 = 45_s;
	cout << t3.str() << '\n';

	auto t4 = "12:05:37"_timestamp;
	cout << t4.str() << '\n';

	return 0;
}

