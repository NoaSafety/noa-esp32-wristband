#ifndef BUTTONSENSOR_INCLUDED
#define BUTTONSENSOR_INCLUDED

class ButtonSensor
{
    public:
        ButtonSensor(StateManager& state, int pin) :
            m_state(state),
            m_pin(pin)
        {
            pinMode(m_pin, INPUT_PULLUP);
        }

        void checkButton()
        {
            auto btnState = digitalRead(m_pin);
            if(btnState == LOW && btnState != m_prevBtnState)
                m_state.toggleSOSMode();
            m_prevBtnState = btnState;
        }

    private:
        int m_pin;
        int m_prevBtnState;
        StateManager& m_state;
};

#endif
