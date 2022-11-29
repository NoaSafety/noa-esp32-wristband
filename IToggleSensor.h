#ifndef ITOGGLESENSOR_INCLUDED
#define ITOGGLESENSOR_INCLUDED

#include <memory>

class IToggleSensor : public std::enable_shared_from_this<IToggleSensor>
{
    public:
        virtual ~IToggleSensor() = default;
        virtual void enable(bool ena) = 0;
        virtual void toggle() = 0;
        virtual void update() = 0;
};

#endif
