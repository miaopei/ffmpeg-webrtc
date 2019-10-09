#include <iostream>

#include "FighterPlane.h"


int main(int argc, char* argv[])
{

    avdance::FighterPlane fp;
    fp.setWings(15);
    fp.setWheels(3);
    fp.setWeapons(1);

    std::cout<< "FighterPlane:" << fp.getWings() << ", "
                                 << fp.getWheels() << ", "
                                 << fp.getWeapons() << std::endl;

}
