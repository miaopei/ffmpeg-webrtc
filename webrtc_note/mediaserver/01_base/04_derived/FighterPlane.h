#ifndef __CLIENT_AIRPLAN_H__
#define __CLIENT_AIRPLAN_H__

#include <iostream>
#include "AirPlane.h"

namespace avdance {

class FighterPlane: public AirPlane {
    public:
        FighterPlane() {
            weapons = 1; 
        }

        ~FighterPlane(){
        
        }

    public:
        void setWeapons(int w);
        int getWeapons();

    private:
        int weapons;
};

} //namesapce avdance

#endif //__CLIENT_AIRPLAN_H__
