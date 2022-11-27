#ifndef LEDSENSOR_INCLUDED
#define LEDSENSOR_INCLUDED

#include "IToggleSensor.h"

class LedSensor : public IToggleSensor
{
    public:
        LedSensor(int pin) :
            m_pin(pin),
            m_on(false),
            m_enabled(false)
        {
            pinMode(m_pin, OUTPUT);
        }

        void enable(bool ena) override
        {
            if(!(m_enabled = ena))
                digitalWrite(m_pin, LOW);
        }

        void toggle() override
        {
            enable(!m_enabled);
        }
        
        void update() override
        {
            if(m_enabled)
                digitalWrite(m_pin, HIGH);
        }

    private:
        int m_pin;
        bool m_on;
        bool m_enabled;
};

#endif
