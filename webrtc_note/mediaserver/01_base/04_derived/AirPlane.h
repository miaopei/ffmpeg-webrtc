#ifndef __AIRPLANE_H__
#define __AIRPLANE_H__

#include <iostream>

namespace avdance {

class AirPlane{
    public:
        AirPlane(){}
        ~AirPlane(){}

    public:
        void setWings(int w);
        int getWings();

        void setWheels(int w);
        int getWheels();

    private:
        int wings;
        int wheels;
};

} //namespace avdance

#endif //__AIRPLAN_H__
