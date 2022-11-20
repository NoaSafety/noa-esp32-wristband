#ifndef LORATRANSMITTER_INCLUDED
#define LORATRANSMITTER_INCLUDED

#include <SPI.h>
#include <LoRa.h>

class LoRaTransmitter
{
    public:
        LoRaTransmitter(int band, int sck, int miso, int mosi, int ss, int rst, int dio0) :
            m_band(band),
            m_sck(sck),
            m_miso(miso),
            m_ss(ss),
            m_rst(rst),
            m_dio0(dio0)
        {
            
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
            LoRa.beginPacket();
            LoRa.print(data);
            LoRa.endPacket();
        }

    private:
        int m_band;
        int m_sck;
        int m_miso;
        int m_mosi;
        int m_ss;
        int m_rst;
        int m_dio0;
};

#endif
