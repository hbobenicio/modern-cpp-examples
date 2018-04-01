#include <iostream>

struct Foo {
    int x;
    
    Foo(int x) : x(x) { }
    // explicit Foo(int x) : x(x) { }
};

static void printFoo(Foo foo) {
    std::cout << foo.x << '\n';
}

int main() {

    // Implicit cast (without using the 'exclicit' keyword.
    // When using 'explicit' on the constructor, this will fail compiling.
    printFoo(42);
}

