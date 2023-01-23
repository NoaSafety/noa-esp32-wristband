#ifndef MIC_INCLUDED
#define MIC_INCLUDED

class MicSensor
{
    public:
        MicSensor(int pin) :
            m_pin(pin),
            m_prevMicState(LOW)
        {
            pinMode(m_pin, INPUT);
        }

        void checkMic()
        {
            /*auto micState = digitalRead(m_pin);
            update(micState);
            
            m_prevMicState = micState;*/
            int volume = (float) analogRead(m_pin) / (float) 1023 * 100; //calculate percentage of max analogue
            auto micState = volume > 75 ? LOW : HIGH;
            update(micState);
            m_prevMicState = micState;
            
            m_lastEvent = millis();
        }

        void setOnSound(std::function<void(void)> onSound)
        {
            m_onSound = std::move(onSound);
        }

    private:
        int m_pin;
        int m_prevMicState;
        unsigned long m_lastEvent { 0 };
        std::function<void(void)> m_onSound;
        unsigned long m_eventLength { 0 };
        // const unsigned long m_threshold { 500 };
        const unsigned long m_threshold { 3 };
        int m_ticks = 0;

        void update(int state)
        {
            unsigned long currentTime = millis();
            if(state == LOW)
            {
                if(m_prevMicState == state)
                {
                    if(m_lastEvent == 0)
                        return;

                    m_ticks ++;
                    if(m_ticks == m_threshold)
                    {
                        Serial.println("Reached");
                        onSwitchState(true);
                    }
                }
            }
            else
            {
                if(m_prevMicState != state)
                {
                    onSwitchState(false);
                }
            }
        }

        void onSwitchState(bool sound)
        {
            Serial.println("SwitchState");
            if(sound)
            {
                Serial.println("Sound !");
                if(m_onSound)
                    m_onSound();
            }

            m_ticks = 0;
        }
};

#endif
