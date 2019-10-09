#include <iostream>

class Base {
    public:
        Base(int a, int b){
            this->a = a;
            this->b = b; 
        }

    public:
        int a;
        int b;
};


int main(int argc, char* argv[])
{

    const Base *b = new Base(1, 2);

    std::cout<< "Base: a=" << b->a << ", b="
                           << b->b << std::endl;
    //b->a = 4; 报错
    //b->b = 5; 报错

    Base *pb = const_cast<Base*>(b);

    pb->a = 3;
    pb->b = 4;


    std::cout<< "Base: a=" << pb->a << ", b="
                           << pb->b << std::endl;

    return 0;

}
