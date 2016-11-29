#pragma once

namespace bryce_tsp
{
    class RouteNode
    {
    public:
        RouteNode(int id, int pid1, int pid2);
        ~RouteNode();

        bool marked = false;

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

        // Stores whether this node's polyline should be interpreted forwards, or backwards.
        
        bool flipped = false;
        int index_start;
        int index_end;

        // Circular Doubly Linked List.
        // Next pointers are consistent.
        RouteNode * next;
        RouteNode * prev;

        // Flips this node.
        void flip();// Flips starting and ending points, but keeps location in domino list.

        // Reverses orientation in domino list. Maintains the connections between paired points.
        void reverse();
    };
}