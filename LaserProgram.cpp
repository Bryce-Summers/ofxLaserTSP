#include "LaserProgram.h"

namespace laser
{
    
    LaserProgram::LaserProgram(Route * route)
    {
        this -> route = route;
    }


    LaserProgram::~LaserProgram()
    {
        delete this -> route;
    }

    Route * LaserProgram::getRoute()
    {
        // Copy the private route into one that we can pass permission to the caller.
        
        return copy(route);
    }

    Program * LaserProgram::getCommandList()
    {
        // FIXME:
        cout << "Implement LaserProgram::getCommandList() Please.";
        return NULL;
    }


    // -- Optimization.


    // -- Instructs the LaserProgram to Optimize its route.
    // Updates the internal route pointer to a new route.
    // It frees the original route.
    void LaserProgram::optimize()
    {
        // For deletion later.
        Route * old_route = route;

        RouteOptimizer optimizer(this->route);
    }



}