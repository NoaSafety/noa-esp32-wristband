#ifndef MESSAGE_INCLUDED
#define MESSAGE_INCLUDED

#include <stdarg.h>
#include "ISerializable.h"
#include "Position.h"

class Message : public ISerializable
{
    public:
        Message(const Position& position) : m_position(position)
        {
          
        }
        
        String to_json() override
        {
            char data[128];
            sprintf(data, "{\"longitude\":%f,\"latitude\":%f}", m_position.longitude(), m_position.latitude());
            return data;
        }

    private:
        const Position &m_position;
};

#endif
