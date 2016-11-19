#include "RouteOptimizer.h"

namespace laser
{

    RouteOptimizer::RouteOptimizer(Route * route)
    {
        this -> route = route;
    }



    RouteOptimizer::~RouteOptimizer()
    {
    }

    Route * RouteOptimizer::optimize()
    {
        return copy(this -> route);
    }
}