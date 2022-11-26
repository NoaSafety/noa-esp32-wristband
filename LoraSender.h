#ifndef LORASENDER_INCLUDED
#define LORASENDER_INCLUDED

#include <SPI.h>
#include <LoRa.h>
#include "Message.h"

class LoRaSender
{
    public:
        LoRaSender(StateManager& state, int band, int sck, int miso, int mosi, int ss, int rst, int dio0) :
            m_state(state),
            m_band(band),
            m_sck(sck),
            m_miso(miso),
            m_ss(ss),
            m_rst(rst),
            m_dio0(dio0)
        {
            
        }

        void takeSPILead()
        {
            SPI.end();
            SPI.begin(m_sck, m_miso, m_mosi, m_ss);
        }

        bool begin()
        {
            // SPI LoRa Pins
            SPI.begin(m_sck, m_miso, m_mosi, m_ss);
            // Setup LoRa transceiver module
            LoRa.setPins(m_ss, m_rst, m_dio0);
          
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

        void checkLoRa()
        {
            if(m_state.isSOSMode()) 
            {
                Serial.println(m_state.getUserId());
                Message msg(m_state.getPosition(), m_state.getUserId());
                auto data = msg.to_json();
                write(data);
            }
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
};

#endif
