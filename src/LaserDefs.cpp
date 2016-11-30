#include "LaserDefs.h"

namespace bryce_tsp
{
    Route * copy(Route * route)
    {
        Route * output = new Route();
        for (auto iter = route->begin(); iter != route->end(); iter++)
        {
            output->push_back(copy_polyline(*iter));
        }

        return output;
    };

    Polyline * copy_polyline(Polyline * route)
    {
        Polyline * output = new Polyline();
        for (auto iter = route->cbegin(); iter != route->cend(); iter++)
        {
            output->push_back(*iter);
        }

        return output;
    };

    Polyline * of_polyline_to_polyline(ofPolyline * route)
    {
        Polyline * output = new Polyline();
        for (auto iter = route->begin(); iter != route->end(); iter++)
        {
            output->push_back(*iter);
        }

        return output;
    };

    Polyline * reverse_polyline(Polyline * route)
    {
        Polyline * output = new Polyline();

        for (auto iter = route->rbegin(); iter != route->rend(); ++iter)
        {
            output->push_back(*iter);
        }

        return output;
    };

    // Free an allocated Route.
    void free_route(Route * route)
    {
        // Delete all of the allocated polylines.
        for (auto iter = route->begin(); iter != route->end(); ++iter)
        {
            delete *iter;
        }

        // delete the route.
        delete route;
    };
}