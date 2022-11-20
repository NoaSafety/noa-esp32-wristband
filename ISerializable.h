#ifndef ISERIALIZABLE_INCLUDED
#define ISERIALIZABLE_INCLUDED

class ISerializable
{
    public:
        virtual ~ISerializable() = default;
        virtual String to_json() = 0;
};

#endif
