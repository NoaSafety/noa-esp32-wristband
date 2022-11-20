#ifndef POSITION_INCLUDED
#define POSITION_INCLUDED

#include <inttypes.h>
#include <SoftwareSerial.h>
#include "IDataFragment.h"
//#include <TinyGPS++.h>

class Position : public IDataFragment
{        
    public:
        Position(uint8_t rxPin, uint8_t txPin, double latitude, double longitude) :
            m_ss(rxPin, txPin),
            m_rxPin(rxPin),
            m_txPin(txPin),
            m_latitude(latitude),
            m_longitude(longitude)
        {
            
        }
        
        Position(const Position& pos) :
            m_ss(pos.m_rxPin, pos.m_txPin),
            m_rxPin(pos.m_rxPin),
            m_txPin(pos.m_txPin),            
            m_latitude(pos.m_latitude),
            m_longitude(pos.m_longitude)
        {
            
        }
        
        bool begin() 
        {
            clear_buffer();
            m_ss.begin(9600);  
            return true;
        }
        
        inline double longitude() const { return m_longitude; }
        inline double latitude() const { return m_latitude; }
        
        void update() override
        {
            while(m_ss.available())
            {
                m_buffer[m_messageLength++] = m_ss.read();
                if(m_messageLength == 128)
                    break;

                Serial.write(m_buffer, m_messageLength);
                clear_buffer();
            }
        }

    private:
        void clear_buffer()
        {
            for(auto i = 0; i < m_messageLength; i ++)
            {
                m_buffer[i] = '\0';
            }

            m_messageLength = 0;
        }
        
        SoftwareSerial m_ss;
        uint8_t m_rxPin;
        uint8_t m_txPin;
        double m_latitude;
        double m_longitude;
        unsigned char m_buffer[128];
        size_t m_messageLength { 128 };
        //TinyGPSPlus m_gps;
};

/*class Position : public IDataFragment
{
    public:        
        Position(int rx_pin, int tx_pin) : m_ss(rx_pin, tx_pin) // RX, TX
        {
           
        }

        bool begin() 
        {
            clear_buffer();
            m_ss.begin(9600);  
            return true;
        }
        
        inline double longitude() const { return m_longitude; }
        inline double latitude() const { return m_latitude; }
        
        void update() override
        {
            while(m_ss.available())
            {
                m_buffer[m_messageLength++] = m_ss.read();
                if(m_messageLength == 128)
                    break;

                Serial.write(m_buffer, m_messageLength);
                clear_buffer();
            }
        }

    private:
        void clear_buffer()
        {
            for(auto i = 0; i < m_messageLength; i ++)
            {
                m_buffer[i] = '\0';
            }

            m_messageLength = 0;
        }
        
    private:
        double m_latitude { 50.62021924615207 };
        double m_longitude { 5.582367411365839 };
        unsigned char m_buffer[128];
        size_t m_messageLength { 128 };
        //TinyGPSPlus m_gps;
        SoftwareSerial m_ss;
};*/

#endif
