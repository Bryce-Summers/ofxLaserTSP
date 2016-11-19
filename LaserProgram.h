#pragma once

#include "ofMain.h";
#include "LaserDefs.h";
#include "RouteOptimizer.h";
#include "RouteNode.h";

#include <vector>;

namespace laser
{

    // Laser Commands tell the robot to turn on the laser, turn off the laser,
    // or move the laser to a new location.
    // they will then contain a relevant ofPoint if it is a move command.
    enum LaserCommandType
    {
        ON, OFF, MOVETO
    };

    class LaserCommand
    {
        LaserCommandType type;
        ofPoint pt;
    };

    typedef std::vector<LaserCommand> Program;

    class LaserProgram
    {

    private:
        // Private Route object.
        Route * route;

    public:
        // ASSUMES: route allocated via new. The Laser Program now owns the input route and will deal with deallocation.
        LaserProgram(Route * route);
        ~LaserProgram();

        // -- Instructs the LaserProgram to Optimize its route.
        // Updates the internal route pointer to a new route.
        // It frees the original route.

        // FIXME: I would like to add some inputs for maximum time or maximum distance.
        // I could add a boolean return if it meets the input requirements.
        void optimize();

        // Returns the current route. This will be an optimized route if the optimize function has been used.
        Route * getRoute();

        Program * getCommandList();

    // Optimization functions.
    private:

        // A local allocation of memory for all of the route nodes,
        // which constitute a modifiable linked list.
        std::vector<RouteNode> nodes; // Stored in by index, nodes[i].id = i;
        std::vector<ofPoint>   points;  // Stored in by index, points[nodex[i].start]

        // Flips node i if it decreases the global metric.
        bool attemptFlip(int i);

        // Attempts to point i1 to i2 and 
        bool attemptSwap(int i1, int i2)

        // global metric.
        // Returns a consistent for how long the path is.
        // We wish to minimize this value.
        float metric(std::vector<RouteNode> nodes);

        // Returns a consistent heuristic for the length of a path from id1 to id2.
        float metric(int id1, int id2);

        // Returns a consistent heuristic for the length of a path, with a penalty for sharp angles.
        // float metric(int id1_p, int id1, int id2, int id2_p);
    };
}