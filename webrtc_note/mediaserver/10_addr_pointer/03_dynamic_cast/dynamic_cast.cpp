#include <iostream>

class Base {
    public:
        Base(int a, int b){
            this->a = a; 
            this->b = b;
        }
    public:
        virtual void print(){
            std::cout << "member: a=" << a << ", b="
                                      << b << std::endl;
        }
    public:
        int a;
        int b;
};

class Derived1: public Base {
    public:
        Derived1(int a, int b, int o):Base(a, b){
            c = o; 
        }
    public:
        virtual void print(){
            std::cout << "member: a=" << a << ", b="
                                      << b << ", c="
                                      << c << std::endl;
        }
    public:
        int c;
};

class Derived2: public Base {
    public:
        Derived2(int a, int b, int o):Base(a, b){
           d = o; 
        }
    public:
        virtual void print() {
            std::cout << "member: a=" << a << ", b="
                                      << b << ", d="
                                      << d << std::endl;
        }
    public:
        int d;
};

int main(int argc, char* argv[])
{
    Base *b = new Base(1, 2);

    b = dynamic_cast<Base*>(new Derived1(3, 4, 5));
    b->print();

    b = static_cast<Base*>(new Derived2(6, 7, 8));
    b->print();

}
