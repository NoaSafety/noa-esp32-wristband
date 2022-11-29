#ifndef MESSAGE_INCLUDED
#define MESSAGE_INCLUDED

#include <stdarg.h>
#include "ISerializable.h"

class Message : public ISerializable
{
    public:
        Message(const Position& position, const String& userId) : m_position(position), m_userId(userId)
        {
          
        }
        
        String to_json() override
        {
            char data[128];
            sprintf(data, "{\"userId\":\"%s\",\"longitude\":%f,\"latitude\":%f}", m_userId.c_str(), m_position.Longitude, m_position.Latitude);
            return data;
        }

    private:
        const Position &m_position;
        const String &m_userId;
};

#endif
