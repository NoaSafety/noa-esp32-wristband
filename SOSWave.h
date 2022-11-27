#ifndef SOSWAVE_INCLUDED
#define SOSWAVE_INCLUDED

#include "IDigitalWave.h"

class SOSWave : public IDigitalWave
{
    public:
        SOSWave() = default;
        ~SOSWave() override = default;
        
        bool getSignal(double time) const
        {
            if((8 > time && time > 4) ||
                (16 > time && time > 12) ||
                (24 > time && time > 20) ||
                (40 > time && time > 28) ||
                (52 > time && time > 44) ||
                (68 > time && time > 56) ||
                (76 > time && time > 72) ||
                (84 > time && time > 80) ||
                (92 > time && time > 88)) 
                return true;

            return false;
        }   
};

#endif
