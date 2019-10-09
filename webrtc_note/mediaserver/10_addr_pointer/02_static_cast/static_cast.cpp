#include <iostream>

class Base {
    public:
        int a;
        int b;
};

class Derived: public Base {

    public:
        int c;
        int d;
};

int main(int argc, char* argv[])
{
   Base *b = new Base(); 
   Derived *d = new Derived();

   Base *p = static_cast<Base*>(d);
   p->a = 1;
   p->b = 2;

   std::cout << "Base: " << p->a << ", "
                         << p->b << std::endl;

   Derived *dp = static_cast<Derived*>(b);
   dp->a = 4;
   dp->b = 5;
   dp->c = 6;
   dp->d = 666;

   std::cout << "Derived: " << dp->a << ", "
                         << dp->b << ", "
                         << dp->c << ", "
                         << dp->d << std::endl;


   return 0;

}



