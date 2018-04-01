#include <iostream>
#include <ctime>
#include <sstream>

namespace hbo
{
    using time_unit = int;

    class time
    {
        protected:
            time_unit t{0};

        public:
            time(){}
            time(const std::string& str) {
                int h = std::stoi(str.substr(0, 2));
                int m = std::stoi(str.substr(3, 2));
                this->t = h*60 + m;
            }
            time(const time& other) : t(other.t) {}
            time(time_unit t) : t(t) {}

            time operator-(const time& other) const {
                return time{this->t - other.t};
            }

            std::string str() const {
                std::stringstream ss;
                int h = this->t / 60;
                int m = this->t % 60;
                ss.width(2);
                ss.fill('0');
                ss << h << ':';
                ss.width(2);
                ss.fill('0');
                ss << m;
                return ss.str(); 
            }

            void print() const {
                std::cout << this->str() << '\n';
            }

            time_unit get_time() const { return t; }
    };
}

int main(int argc, char *argv[])
{
    if (argc != 5) {
        std::cerr << "Please, provide 4 time arguments to the program.\n";
        exit(1);
    }

    std::string strTime1{argv[1]};
    std::string strTime2{argv[2]};
    std::string strTime3{argv[3]};
    std::string strTime4{argv[4]};

    hbo::time t1{strTime1};
    hbo::time t2{strTime2};
    hbo::time t3{strTime3};
    hbo::time t4{strTime4};

    std::cout << (t2 - t1).str() << '\n';
    auto diff = t4 - t3;
    diff.print();
}