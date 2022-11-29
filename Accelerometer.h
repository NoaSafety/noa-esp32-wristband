#ifndef ACCELEROMETER_INCLUDED
#define ACCELEROMETER_INCLUDED

#include <Wire.h>
#include <array>

#define AXIS_ADDRESS                (0x68)

char tmp_str[7]; // temporary variable used in convert function

char* convert_int16_to_str(int16_t i) 
{ // converts int16 to string. Moreover, resulting strings will have the same length in the debug monitor.
  sprintf(tmp_str, "%6d", i);
  return tmp_str;
}

class Accelerometer 
{
    public:
        Accelerometer(uint8_t sda_pin, uint8_t scl_pin) : 
            m_sda(sda_pin), 
            m_scl(scl_pin)
        {
        }

        void begin()
        {
            Wire.beginTransmission(AXIS_ADDRESS);
            Wire.write(0x6B); // Power management (1) register
            Wire.write(0x00); // (Wake up)
            Wire.endTransmission(true);
        }

        void update()
        {
            Wire.beginTransmission(AXIS_ADDRESS);
            Wire.write(0x3B); // Acceleration X_OUT_H
            Wire.endTransmission(false); // Send restart and keep connection alive

            // Read data
            auto registers = sizeof(uint16_t) * 7;
            Wire.requestFrom(AXIS_ADDRESS, registers, true);

            m_accelerometer[0] = Wire.read() << 8 | Wire.read();
            m_accelerometer[1] = Wire.read() << 8 | Wire.read();
            m_accelerometer[2] = Wire.read() << 8 | Wire.read();
            m_temperature = Wire.read() << 8 | Wire.read();
            m_gyro[0] = Wire.read() << 8 | Wire.read();
            m_gyro[1] = Wire.read() << 8 | Wire.read();
            m_gyro[2] = Wire.read() << 8 | Wire.read();

            Serial.print("aX = "); Serial.print(convert_int16_to_str(m_accelerometer[0]));
            Serial.print(" | aY = "); Serial.print(convert_int16_to_str(m_accelerometer[1]));
            Serial.print(" | aZ = "); Serial.print(convert_int16_to_str(m_accelerometer[2]));
            Serial.print(" | tmp = "); Serial.print(m_temperature/340.00+36.53);
            Serial.print(" | gX = "); Serial.print(convert_int16_to_str(m_gyro[0]));
            Serial.print(" | gY = "); Serial.print(convert_int16_to_str(m_gyro[1]));
            Serial.print(" | gZ = "); Serial.print(convert_int16_to_str(m_gyro[2]));
            Serial.println();
        }

    private:
        uint8_t m_sda;
        uint8_t m_scl;

        std::array<int16_t, 3> m_accelerometer;
        std::array<int16_t, 3> m_gyro;
        int16_t m_temperature;
}

#endif
