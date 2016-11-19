#pragma once

namespace laser
{
    class RouteNode
    {
    public:
        RouteNode(int id, int pid1, int pid2);
        ~RouteNode();

        bool marked = false;
        bool start = true;

        // A route is assumed to consist of "domino" nodes linked between each other.
        // These nodes contain indices into point data arrays.
        // Consider the route A -> B -> C
        // The polylines would be:
        // 1. A.start -> A.end,
        // 2. A.end   -> B.start,
        // 3. B.start -> B.end,
        // 4. B.end   -> C.start,
        // 5. C.start -> C.end,
        // 6. C.end   -> A.start.
        int id;
        int index_start;
        int index_end;
    };
}