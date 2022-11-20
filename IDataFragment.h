#ifndef IDATAFRAGMENT_INCLUDED
#define IDATAFRAGMENT_INCLUDED

class IDataFragment
{
    public:
        virtual ~IDataFragment() = default;
        virtual void update() = 0;
};

#endif
