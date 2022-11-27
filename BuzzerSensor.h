#ifndef BUZZERSENSOR_INCLUDED
#define BUZZERSENSOR_INCLUDED

#include "IDigitalWave.h"
#include "IToggleSensor.h"

class BuzzerSensor : public IToggleSensor
{
    public:
        BuzzerSensor(int pin, IDigitalWave& soundWave) :
            m_pin(pin),
            m_soundWave(soundWave),
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
                digitalWrite(m_pin, getStateFromTime(millis()) ? HIGH : LOW);
        }

    private:
        int m_pin;
        IDigitalWave& m_soundWave;
        bool m_enabled;

        bool getStateFromTime(unsigned long t) const
        {
            auto length = 4000;
            auto moment = (int) (100 * (((double)(t % length)) / (double)length));

            return m_soundWave.getSignal(moment);
        }
};

#endif
