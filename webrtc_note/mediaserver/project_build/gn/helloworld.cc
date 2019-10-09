#include <iostream>

class Hello{

  public:
    Hello(){};
    ~Hello(){};

  public:
    void setA(int aa){
      a = aa; 
    }

    int getA(){
      return a; 
    }

  private:
    int a;
};

int main(int argc, char* argv[])
{
  Hello *h = new Hello();
  h->setA(90);

  std::cout << "a equals :" << h->getA() << std::endl;
}
