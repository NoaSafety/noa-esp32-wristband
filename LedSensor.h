#ifndef LEDSENSOR_INCLUDED
#define LEDSENSOR_INCLUDED

class LedSensor
{
    public:
        LedSensor(StateManager& state, int pin) :
            m_state(state),
            m_pin(pin)
        {
            pinMode(m_pin, OUTPUT);
        }

        void checkLed()
        {
            digitalWrite(m_pin, m_state.isSOSMode() ? HIGH : LOW);
        }

    private:
        int m_pin;
        StateManager& m_state;
};

#endif
