#include <iostream>

#include "AirPlane.h"

namespace avdance {

void AirPlane::setWings(int w){
    wings = w;
}

int AirPlane::getWings(){
    return wings;
}

void AirPlane::setWheels(int w){
    wheels = w;
}

int AirPlane::getWheels(){
    return wheels;
}

} //namespace avdance
