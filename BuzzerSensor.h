#ifndef BUZZERSENSOR_INCLUDED
#define BUZZERSENSOR_INCLUDED

class BuzzerSensor
{
    public:
        BuzzerSensor(StateManager& state, int pin) :
            m_state(state),
            m_pin(pin)
        {
            pinMode(m_pin, OUTPUT);
        }

        void checkBuzzer()
        {
            if(m_state.isSOSMode()) 
            {
                digitalWrite(m_pin, HIGH);
                delay(500);
                digitalWrite(m_pin, LOW); 
            }
        }

    private:
        int m_pin;
        StateManager& m_state;
};

#endif
