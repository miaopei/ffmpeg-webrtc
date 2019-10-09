/**
 * for testing class and use it
 * 
 * @author lichao
 * @date 2019-08-10
 * @copyleft GPL 2.0
 */

#include <iostream>

#include "Human.h"

using namespace avdance;

int main(int argc, char* argv[])
{
   Human* human = new Human(); 
   human->setAge(28);
   human->setSex(1);
    
    std::cout << "human:" << human->getAge() << ", " << human->getSex() << std::endl;

}
