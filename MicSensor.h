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
            auto micState = digitalRead(m_pin);
            update(micState);
            
            m_prevMicState = micState;
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
        const unsigned long m_threshold { 500 };

        void update(int state)
        {
            auto currentTime = millis();
            if(state == LOW)
            {
                if(m_prevMicState == state)
                {
                    if(m_lastEvent == 0)
                        goto end;
                        
                    m_eventLength += currentTime - m_lastEvent;
                    Serial.println("Event len " + m_eventLength);

                    if(m_eventLength > m_threshold)
                    {
                        onSwitchState(LOW);
                    }
                }
            }
            else
            {
                if(m_prevMicState != state)
                {
                    onSwitchState(HIGH);
                }
            }

            end:
            m_lastEvent = millis();
        }

        void onSwitchState(int state)
        {
            if(state == LOW)
            {
                Serial.println("Toggle");
                if(m_onSound)
                    m_onSound();
            }

            m_eventLength = 0;
        }
};

#endif
