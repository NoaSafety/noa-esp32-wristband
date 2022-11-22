#ifndef POSITION_INCLUDED
#define POSITION_INCLUDED

#include <inttypes.h>
#include <SoftwareSerial.h>
#include "IDataFragment.h"
#include <VMA430_GPS.h>
#include <TinyGPS++.h>

#define GPS_BAUDRATE            (9600)

class Position : public IDataFragment
{        
    public:
        Position(uint8_t rxPin, uint8_t txPin, double latitude, double longitude) :
            //m_ss(rxPin, txPin),
            m_rxPin(rxPin),
            m_txPin(txPin),
            m_latitude(latitude),
            m_longitude(longitude),
            m_gps(),
            m_ss(rxPin, txPin)
        {
            
        }
        
        Position(const Position& pos) :
            m_ss(pos.m_rxPin, pos.m_txPin),
            m_rxPin(pos.m_rxPin),
            m_txPin(pos.m_txPin),            
            m_latitude(pos.m_latitude),
            m_longitude(pos.m_longitude),
            m_gps()
        {
            
        }
        
        bool begin() 
        {
            // Serial2.begin(GPS_BAUDRATE, SERIAL_8N1, m_rxPin, m_txPin);
            m_ss.begin(GPS_BAUDRATE);
            /*clear_buffer();
            m_ss.begin(GPS_BAUDRATE);  */
            /*Serial2.begin(GPS_BAUDRATE, SERIAL_8N1, m_rxPin, m_txPin);
            m_gps.begin(GPS_BAUDRATE); // Sets up the GPS module to communicate with the Arduino over serial at 9600 baud
            m_gps.setUBXNav(); // Enable the UBX mavigation messages to be sent from the GPS module*/
            return true;
        }
        
        inline double longitude() const { return m_longitude; }
        inline double latitude() const { return m_latitude; }
        
        void update() override
        {
            /*if(!m_ss.available())
            {
                Serial.print("Nope");
            }
            while(m_ss.available() > 0)
            {
                if(m_gps.encode(m_ss.read()))
                {
                    if(m_gps.location.isValid())
                    {
                        m_latitude = m_gps.location.lat();
                        m_longitude = m_gps.location.lng();
                    }
                }
            }*/
            /*if (m_gps.getUBX_packet()) // If a valid GPS UBX data packet is received...
            {
                m_gps.parse_ubx_data(); // Parse the new data
                m_latitude = m_gps.location.latitude;
                m_longitude = m_gps.location.longitude;
            }*/

            if(m_ss.available() == 0)
            {
                Serial.println("No data found");
            }

            while(m_ss.available() > 0) 
            {//while data is available
                Serial.println("Data found");
                if(m_gps.encode(m_ss.read())) //read gps data
                {
                    if(m_gps.location.isValid()) //check whether gps location is valid
                    {
                        m_latitude = m_gps.location.lat();
                        m_longitude = m_gps.location.lng();
                    }
                }
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
        
        //SoftwareSerial m_ss;
        uint8_t m_rxPin;
        uint8_t m_txPin;
        double m_latitude;
        double m_longitude;
        unsigned char m_buffer[128];
        SoftwareSerial m_ss;
        size_t m_messageLength { 128 };
        TinyGPSPlus m_gps;
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
