#include <iostream>


int main(int argc, char* argv[]){

    int a[2] = { 88, 99 };

    int *p = &a[0];

    unsigned long addr = reinterpret_cast<unsigned long>(p);


    std::cout<< "the value of addr: " << *p << std::endl;

    std::cout << "addr: " << addr << std::endl;

    std::cout << "new addr: " << addr + 4 << std::endl; 

    int *p1 = reinterpret_cast<int*>(addr+4);

    std::cout << "the value of new addr: " << *p1 << std::endl;

    return 0;

}
