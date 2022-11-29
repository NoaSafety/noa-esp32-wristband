#ifndef GPSSENSOR_INCLUDED
#define GPSSENSOR_INCLUDED

#include <SoftwareSerial.h>
#include <VMA430_GPS.h> 
#include <inttypes.h>

#define GPS_BAUDRATE            (9600)

class GPSSensor
{
    public:
        GPSSensor(const GPSSensor& gpsSensor):
            m_state(gpsSensor.m_state),
            m_rx_pin(gpsSensor.m_rx_pin),
            m_tx_pin(gpsSensor.m_tx_pin),
            m_ss(gpsSensor.m_rx_pin, gpsSensor.m_tx_pin),
            m_gps(gpsSensor.m_gps)
        {
          
        }
        
        GPSSensor(StateManager& state, int rx_pin, int tx_pin) :
            m_state(state),
            m_rx_pin(rx_pin),
            m_tx_pin(rx_pin),
            m_ss(rx_pin, tx_pin),
            m_gps(&m_ss)
        {

        }

        bool begin()
        {
            m_gps.begin(GPS_BAUDRATE); // Sets up the GPS module to communicate with the Arduino over serial at 9600 baud
            m_gps.setUBXNav(); // Enable the UBX mavigation messages to be sent from the GPS module
            return true;
        }
        
        void checkGPS()
        {
            /*if (m_gps.getUBX_packet()) // If a valid GPS UBX data packet is received...
            {
                m_gps.parse_ubx_data(); // Parse the new data  
                m_state.setPosition((Position){ .Latitude = m_gps.location.latitude, .Longitude = m_gps.location.longitude});
            }*/
        }

    private:
        int m_rx_pin;
        int m_tx_pin;
        SoftwareSerial m_ss;
        VMA430_GPS m_gps;
        StateManager& m_state;
};

#endif
