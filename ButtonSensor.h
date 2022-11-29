#ifndef BUTTONSENSOR_INCLUDED
#define BUTTONSENSOR_INCLUDED

#include <functional>

class ButtonSensor
{
    public:
        ButtonSensor(int pin) :
            m_pin(pin),
            m_prevBtnState(LOW)
        {
            pinMode(m_pin, INPUT_PULLUP);
        }

        void checkButton()
        {
            auto btnState = digitalRead(m_pin);
            if(btnState == LOW && btnState != m_prevBtnState)
            {
                if(m_onPressed)
                    m_onPressed(btnState);
            }
            
            m_prevBtnState = btnState;
        }

        void setOnPressedListener(std::function<void(bool)> onPressed)
        {
            m_onPressed = std::move(onPressed);
        }

    private:
        int m_pin;
        int m_prevBtnState;
        std::function<void(bool)> m_onPressed;
};

#endif
