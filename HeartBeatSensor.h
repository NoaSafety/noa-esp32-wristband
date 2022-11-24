#include <PulseSensorPlayground.h>

#ifndef HEARTBEATSENSOR_INCLUDED
#define HEARTBEATSENSOR_INCLUDED

#ifndef USE_ARDUINO_INTERRUPTS
    #define USE_ARDUINO_INTERRUPTS true
#endif

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
            m_pulseSensor.setOutputType(SERIAL_PLOTTER);
            m_pulseSensor.setThreshold(m_threshold);
            return m_pulseSensor.begin();
        }

        void update()
        {
            auto test = analogRead(m_pin);
            Serial.print(test);
            Serial.write(" - ");
            m_beat = m_pulseSensor.getBeatsPerMinute();
            Serial.write("Heartbeat: ");
            Serial.println(m_beat);

            if(!m_pulseSensor.sawStartOfBeat())
                m_beat = 0;
        }

        inline int heartbeat()
        {
            return m_beat;
        }
        
    private:
        int m_pin;
        int m_beat;
        int m_threshold;
        PulseSensorPlayground m_pulseSensor;
};

#endif
