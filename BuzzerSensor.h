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
            digitalWrite(m_pin, m_state.isSOSMode() ? HIGH : LOW);
        }

    private:
        int m_pin;
        StateManager& m_state;
};

#endif
