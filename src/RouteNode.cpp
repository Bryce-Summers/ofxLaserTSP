#include "RouteNode.h"

namespace laser
{
    RouteNode::RouteNode(int id, int pid1, int pid2)
    {
        this -> id = id;
        this -> index_start = pid1;
        this -> index_end   = pid2;
    }

    RouteNode::~RouteNode()
    {
    }

    void RouteNode::flip()
    {
        // This is used to signal the output to invert the input polyline.
        flipped = !flipped;

        // swap.
        int temp = index_start;
        index_start = index_end;
        index_end = temp;
    }

    void RouteNode::reverse()
    {
        RouteNode * temp = prev;
        prev = next;
        next = temp;

        flip();
    }
}