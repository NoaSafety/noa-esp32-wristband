#ifndef POSITION_INCLUDED
#define POSITION_INCLUDED

struct Position
{
    Position() : 
        Latitude(0.0), 
        Longitude(0.0)
    {
        
    }

    Position(double latitude, double longitude) : 
        Latitude(latitude), 
        Longitude(longitude)
    {
        
    }

    Position(const Position& position) :
        Latitude(position.Latitude),
        Longitude(position.Longitude)
    {
        
    }
    
    double Latitude;
    double Longitude;
};

#endif
