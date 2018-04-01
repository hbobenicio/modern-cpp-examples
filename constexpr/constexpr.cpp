#include <iostream>

constexpr unsigned long long fibonacci(int n) {
    return (n == 0) ? 1 : n + fibonacci(n - 1);
}

int main(int argc, char* argv[])
{
    int n = atoi(argv[1]);
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 500; j++) {
            fibonacci(170001);
        }
    }
    
}