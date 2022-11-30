#ifndef ACCELEROMETER_INCLUDED
#define ACCELEROMETER_INCLUDED

#include "Wire.h"
#include <array>
#include "CircularBuffer.h"
#include <cmath>
#include <functional>

#define AXIS_ADDRESS                (0x68)
#define MOVEMENT_LENGTH             (20)

class Accelerometer 
{
    public:
        Accelerometer(uint8_t sda_pin, uint8_t scl_pin) : 
            m_sda(sda_pin), 
            m_scl(scl_pin)
        {
            for(auto i = 0; i < MOVEMENT_LENGTH; i ++)
                m_movements.push(0);
        }

        bool begin()
        {
            // Wire.begin(m_sda, m_scl);
            Wire.beginTransmission(AXIS_ADDRESS);
            Wire.write(0x6B); // Power management (1) register
            Wire.write(0x00); // (Wake up)
            Wire.endTransmission(true);

            return true;
        }

        void update()
        {
            Wire.beginTransmission(AXIS_ADDRESS);
            Wire.write(0x3B); // Acceleration X_OUT_H
            Wire.endTransmission(false); // Send restart and keep connection alive

            // Read data
            size_t registers = sizeof(int16_t) * 7;
            Wire.requestFrom(static_cast<uint8_t>(AXIS_ADDRESS), registers, true); // 7 * 2 bytes (int16_t)

            read();
            if(checkMovement() && m_onShaked)
                m_onShaked();
                
            // print();
        }

        void setOnShakedListener(std::function<void(void)> onShaked)
        {
            m_onShaked = std::move(onShaked);
        }

        bool checkMovement()
        {
            auto totalMovement = 0;
            for(auto i = 0; i < MOVEMENT_LENGTH; i ++)
            {
                totalMovement += m_movements.get(i);
            }

            if(totalMovement < 1500000)
            {
                return false;
            }

            m_movements.clear();
            return true;
        }

        void read()
        {
            std::array<int16_t, 3> accelerometer;
            int16_t temperature;
            std::array<int16_t, 3> gyroscope;
            accelerometer[0] = Wire.read() << 8 | Wire.read();
            accelerometer[1] = Wire.read() << 8 | Wire.read();
            accelerometer[2] = Wire.read() << 8 | Wire.read();
            m_temperature = Wire.read() << 8 | Wire.read();
            gyroscope[0] = Wire.read() << 8 | Wire.read();
            gyroscope[1] = Wire.read() << 8 | Wire.read();
            gyroscope[2] = Wire.read() << 8 | Wire.read();

            unsigned int delta = abs(accelerometer[0] - m_accelerometer[0]) + abs(accelerometer[1] - m_accelerometer[1]) + abs(accelerometer[2] - m_accelerometer[2]);
            m_movements.push(delta);
            m_accelerometer = accelerometer;
            m_gyro = gyroscope;
            m_temperature = temperature;
        }

        void print()
        {
            Serial.print("Ax = "); Serial.print(m_accelerometer[0]);
            Serial.print(", Ay = "); Serial.print(m_accelerometer[1]);
            Serial.print(", Az = "); Serial.print(m_accelerometer[2]);
            Serial.print(" | tmp = "); Serial.print(m_temperature / 340.00 + 36.53);
            Serial.print(" | gX = "); Serial.print(m_gyro[0]);
            Serial.print(", gY = "); Serial.print(m_gyro[1]);
            Serial.print(", gZ = "); Serial.print(m_gyro[2]);
            Serial.println();
        }

    private:
        uint8_t m_sda;
        uint8_t m_scl;

        std::array<int16_t, 3> m_accelerometer;
        std::array<int16_t, 3> m_gyro;
        int16_t m_temperature;

        CircularBuffer<unsigned int, MOVEMENT_LENGTH> m_movements;
        std::function<void(void)> m_onShaked;
};

#endif
