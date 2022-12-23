#ifndef OLEDDISPLAY_INCLUDED
#define OLEDDISPLAY_INCLUDED

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "IToggleSensor.h"
#include <cmath>

class OledDisplay : public IToggleSensor
{
    public:
        OledDisplay(StateManager& state, int width, int height, int resetPin, int size, int sda, int scl) : 
            m_state(state),
            m_display(width, height, &Wire, resetPin),
            m_size(size),
            m_lineSize(size * 10),
            m_line(0),
            m_width(width),
            m_height(height),
            m_sda(sda),
            m_scl(scl),
            m_enabled(false)
        {
            
        }

        bool begin(int reset)
        {
            pinMode(reset, OUTPUT);
            digitalWrite(reset, LOW);
            delay(20);
            digitalWrite(reset, HIGH);
  
            Wire.begin(m_sda, m_scl);
            if(m_display.begin(SSD1306_SWITCHCAPVCC, 0x3c, false, false)) // 0x3C -> 128x32
            {
                clear();
                set_default_style();
                return true;
            }
            return false;
        }

        void clear()
        {
            m_display.clearDisplay();
            m_display.setCursor(0, 0);
            m_line = 0;
        }

        void set_default_style()
        {
            m_display.setTextColor(WHITE);
            m_display.setTextSize(m_size);
        }
            
        void push_line(String text)
        {
            clear_line(m_line);
            m_display.setCursor(0, m_lineSize * m_line);
            m_display.print(text);
            m_line ++;
            m_display.display();
        }

        void set_line(int line)
        {
            m_line = line;
            m_display.setCursor(0, m_lineSize * m_line);
        }

        void clear_line(int line)
        {
            for (auto y = m_lineSize * line, yMax = m_lineSize * (line + 1); y <= yMax; y ++)
            {
                for (auto x = 0; x < m_width; x ++)
                {
                    m_display.drawPixel(x, y, BLACK); 
                }
            }
        }

        void update() override
        {
            if(m_loading)
            {
                update_loading();
                return;
            }
            
            set_line(4);
            if(m_enabled)
                push_line("SOS Mode: ON");
            else
                push_line("SOS Mode: OFF");

            set_line(1);
            push_line("UID: " + m_state.getUserId());
        }

        void update_loading()
        {
            static int s = 0;
            /* auto sliceSize = 208; // ms
            auto normalizedMoment = millis() % 4992;
            auto slice = normalizedMoment / sliceSize; */
            auto pi = 3.141592;
            auto distance = m_height * 0.45;
            auto xCenter = m_width / 2;
            auto yCenter = m_height / 2;
            clear();

            for(auto i = s; i < s + 5; i ++)
            {
                /* auto s = i % 24;
                if(s < 0)
                    s += 24; */
                    
                auto angle = (2 * pi) / i;
                auto x = xCenter + (distance * sin(angle));
                auto y = yCenter + (distance * cos(angle));

                // Serial.println(i);

                m_display.drawPixel(x, y, WHITE); 
            }
            m_display.display();

            if(++s == 24)
                s = 0;
            
        }

        void setLoading(bool toggle)
        {
            m_loading = toggle;
            clear();
        }

        void enable(bool ena) override
        {
            m_enabled = ena;
        }

        void toggle() override
        {
            enable(!m_enabled);
        }
        
    private:
        Adafruit_SSD1306 m_display;
        StateManager& m_state;
        bool m_loading { false };
        int m_size;
        int m_lineSize;
        int m_line;
        int m_width;
        int m_height;
        int m_sda;
        int m_scl;
        bool m_enabled;
};

#endif
