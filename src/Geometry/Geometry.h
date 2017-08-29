#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <vector>

#include "Params.h"

class Geometry
{
public:
    Geometry( Params& params ) {};
    virtual ~Geometry( ) {};

    virtual unsigned int getDomainId( std::vector<unsigned int> Coordinates ) = 0;
    virtual std::vector<unsigned int> getDomainCoordinates( unsigned int Id ) = 0;
};

#endif

