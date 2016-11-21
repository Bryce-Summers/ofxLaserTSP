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
        Program * program = new Program();

        // Instructs the artist to turn the laser off.
        LaserCommand off;
        off.type = OFF;

        // Instructs the artist to turn the laser on for drawing.
        LaserCommand on;
        on.type = ON;

        int index = 0;
        for (auto iter = route -> cbegin(); iter != route -> cend(); ++iter)
        {
            int ID = path_permutation[index];
            index++;

            // Populate the off command.
            off.ID = ID;
            program -> push_back(off);
           
            Polyline * polyline = *iter;

            auto point_iter = polyline -> cbegin();
            LaserCommand move;
            move.type = MOVETO;
            move.pt = *point_iter;
            move.ID = ID;

            // Move without the laser on.
            program -> push_back(move);

            // Turn the laser on.
            on.ID = ID;
            program -> push_back(on);

            // Draw the rest of the movements with the laser on.
            point_iter++;
            while (point_iter != polyline -> cend())
            {
                move.pt = *point_iter;
                program -> push_back(move);
                point_iter++;
            }
        }

        return program;
    }


    // -- Optimization.


    // -- Instructs the LaserProgram to Optimize its route.
    // Updates the internal route pointer to a new route.
    // It frees the original route.
    void LaserProgram::optimize()
    {
        // For deletion later.
        Route * old_route = route;

        RouteOptimizer optimizer(this -> route);
        this -> route = optimizer.optimize();

        // Get the permutation for concerned artists who want their original colors.
        optimizer.populatePermutation(path_permutation);

        delete old_route;
    }



}