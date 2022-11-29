#ifndef IWAVE_INCLUDED
#define IWAVE_INCLUDED

#include <memory>

class IDigitalWave : public std::enable_shared_from_this<IDigitalWave>
{
    public:
        virtual ~IDigitalWave() = default;
        virtual bool getSignal(double time) const = 0; // Time in percents
};

#endif
