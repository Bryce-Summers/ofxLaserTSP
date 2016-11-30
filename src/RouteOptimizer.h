#pragma once

#include <vector>
#include "ofMain.h"
#include "LaserDefs.h"
#include "RouteNode.h"

/*
 * Polyline Route Optimizer.
 * Written by Bryce Summers on 11/18/2016.
 */

namespace bryce_tsp
{

    class RouteOptimizer
    {

    private:
        Route * route;
        bool closed;

        // Represents the index of the start of the linked list.
        // This is important for non closed solutions, so that they can align the permutation correctly.
        // This is changed when a route is returned to a user.
        int start_index;

    public:
        RouteOptimizer(Route * route, bool closed = true);
        ~RouteOptimizer();

        // Returns a heuristically optimized route.
        // This is guranteed to be a new route in memory.
        Route * optimize(int passes);

        // Permutes the given permutation by the permutation constructed by this RouteOptimizer.
        void permute(std::vector<int> &permutation);

    private:
        // A local allocation of memory for all of the route nodes,
        // which constitute a modifiable linked list.
        std::vector<RouteNode> nodes; // Stored in by index, nodes[i].id = i;
        std::vector<ofPoint>   points;  // Stored in by index, points[nodex[i].start]

                                        // Flips node i if it decreases the global metric.
        bool attemptFlip(int i);

        // Attempts to rearrange the pointer for node i1 -> i2.
        bool attemptSwap(int i1, int i2);

        // global metric.
        // Returns a consistent for how long the path is.
        // We wish to minimize this value.
        float metric(std::vector<RouteNode> nodes);

        // This is where I design the macro optimization passes.
        void aplyOptimizationPasses(int passes);

        // Returns a consistent heuristic for the length of a path from points[id1] to points[id2].
        float metric(int id1, int id2);

        // returns the index of the node where the longest edge points finishes in the forward ordering.
        // The return value will indicate the proper start_index for open problems.
        int getLongestEdgeIndex(std::vector<RouteNode> nodes);

        // Returns a consistent heuristic for the length of a path, with a penalty for sharp angles.
        // float metric(int id1_p, int id1, int id2, int id2_p);

        // Converts from this RouteOptimizer's internal structures back to a route representation.
        // User is responsible for managing the returned memory.
        Route * toRoute();

    };

}