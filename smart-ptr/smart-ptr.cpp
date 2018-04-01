#include <iostream>
#include <memory>

using namespace std;

class Person {
public:
    string name;
    int age;

    Person() { cout << "Person::Person()" << endl; }
    virtual ~Person() { cout << "Person::~Person()" << endl; }
};

unique_ptr<Person> createPerson() {
    unique_ptr<Person> p = make_unique<Person>();
    p.get()->name = "Foo";
    p.get()->age = 29;

    return p;
}

int main() {
    auto p = createPerson();
    auto c = move(p);
    

    return 0;
}
