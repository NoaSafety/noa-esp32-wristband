#ifndef LORASENDER_INCLUDED
#define LORASENDER_INCLUDED

#include <SPI.h>
#include <LoRa.h>
#include "Message.h"
#include "IToggleSensor.h"

class LoRaSender : public IToggleSensor
{
    public:
        LoRaSender(StateManager& state, int band, int sck, int miso, int mosi, int ss, int rst, int dio0) :
            m_state(state),
            m_band(band),
            m_sck(sck),
            m_miso(miso),
            m_mosi(mosi),
            m_ss(ss),
            m_rst(rst),
            m_dio0(dio0),
            m_enabled(false)
        {
            
        }

        void takeSPILead()
        {
            SPI.end();
            SPI.begin(m_sck, m_miso, m_mosi, m_ss);
            LoRa.setPins(m_ss, m_rst, m_dio0);
            LoRa.begin(m_band);
        }

        bool begin()
        {
            takeSPILead();
            return LoRa.begin(m_band);
        }

        void write(String data)
        {
            takeSPILead();
            LoRa.beginPacket();
            LoRa.print(data);
            LoRa.endPacket();
            Serial.println("[LORA] " + data);
        }

        void update() override
        {
            if(m_enabled) 
            {
                Serial.println(m_state.getUserId());
                Message msg(m_state.getPosition(), m_state.getUserId());
                auto data = msg.to_json();
                write(data);
            }
        }
        
        void enable(bool ena) override
        {
            m_enabled = ena;
        }
        
        virtual void toggle() override
        {
            enable(!m_enabled);
        }

    private:
        int m_band;
        int m_sck;
        int m_miso;
        int m_mosi;
        int m_ss;
        int m_rst;
        int m_dio0;
        StateManager& m_state;
        bool m_enabled;
};

#endif
