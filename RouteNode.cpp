#include "RouteNode.h"

namespace laser
{
    RouteNode::RouteNode(int id, int pid1, int pid2)
    {
        this->id = id;
        this->index_start = pid1;
        this->index_end = pid2;
    }

    RouteNode::~RouteNode()
    {
    }
}