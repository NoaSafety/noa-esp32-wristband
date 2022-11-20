#ifndef HEARTBEATSENSOR_INCLUDED
#define HEARTBEATSENSOR_INCLUDED

#define USE_ARDUINO_INTERRUPTS true
#include <PulseSensorPlayground.h>

class HeartBeatSensor
{
    public:
        HeartBeatSensor(int pin, int threshold = 550) :
            m_pin(pin),
            m_threshold(threshold)
        {
        
        }

        bool begin()
        {
            m_pulseSensor.analogInput(m_pin);
            /*m_pulseSensor.blinkOnPulse(PULSE_BLINK);
            m_pulseSensor.fadeOnPulse(PULSE_FADE);*/
            
            m_pulseSensor.setSerial(Serial);
            m_pulseSensor.setOutputType(SERIAL_PLOTTER);
            m_pulseSensor.setThreshold(m_threshold);
            return true;
        }

        void output()
        {
            m_pulseSensor.outputSample();
        }

        void output_beat()
        {
            if(m_pulseSensor.sawStartOfBeat())
            {
                m_pulseSensor.outputBeat();
            }
        }

        inline int read()
        {
            // return analogRead(m_pin);
            return m_pulseSensor.getLatestSample();
        }
        
    private:
        int m_pin;
        int m_threshold;
        PulseSensorPlayground m_pulseSensor;
};

#endif
