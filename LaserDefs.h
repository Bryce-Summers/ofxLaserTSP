#pragma once

/*
* A series of Typedefs in one place to make the Type descriptions better.
* This also imports all of the Laser .h files.
* Written by Bryce Summers on 11/28/2016.
*/
namespace laser
{
    typedef std::vector<ofPoint> Polyline;
    typedef std::vector<Polyline *> Route;

    inline Route * copy(Route * route)
    {
        Route * output = new Route();
        for (auto iter = route -> cbegin(); iter != route -> cend(); iter++)
        {
            output->push_back(*iter);
        }

        return output;
    }
    

}

