#pragma once

#include <vector>
#include "ofMain.h"
#include "LaserDefs.h"

/*
 * Polyline Route Optimizer.
 * Written by Bryce Summers on 11/18/2016.
 */

namespace laser
{

    class RouteOptimizer
    {

    private:
        Route * route;

    public:
        RouteOptimizer(Route * route);
        ~RouteOptimizer();

        // Returns a heuristically optimized route.
        // This is guranteed to be a new route in memory.
        Route * optimize();
    };

}