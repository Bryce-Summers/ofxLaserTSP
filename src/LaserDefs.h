#pragma once

#include <vector>
#include "ofMain.h"

/*
* A series of Typedefs in one place to make the Type descriptions better.
* This also imports all of the Laser .h files.
* Written by Bryce Summers on 11/28/2016.
*/
namespace bryce_tsp
{
    typedef std::vector<ofPoint> Polyline;
    typedef std::vector<Polyline *> Route;

    Route * copy(Route * route);

    Polyline * copy_polyline(Polyline * route);
    
    Polyline * of_polyline_to_polyline(ofPolyline * route);

    Polyline * reverse_polyline(Polyline * route);

    // Free an allocated Route.
    void free_route(Route * route);

}

