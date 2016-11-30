#include "RouteOptimizer.h"
#include <stdexcept>

namespace bryce_tsp
{

    RouteOptimizer::RouteOptimizer(Route * route, bool closed)
    {
        this -> route = route;
        this -> closed = closed;
        int start_index = 0;

        nodes.clear();
        points.clear();

        int id  = 0; // next Route ID.
        int pid = 0; // next Point ID.

        // Allocate all of the Route Nodes and Points.
        for (auto iter = route -> cbegin(); iter != route -> cend(); ++iter)
        {
            RouteNode node = RouteNode(id, pid, pid + 1);
            id  += 1;
            pid += 2;
            nodes.push_back(node);

            Polyline * polyline = *iter;

            points.push_back(polyline -> front());
            points.push_back(polyline -> back());

        }

        // Link up the entire list of nodes.
        int len = nodes.size();
        for (int i = 0; i < len; i++)
        {
            RouteNode & node = nodes[i];
            node.prev = &nodes[(i + len - 1) % len];
            node.next = &nodes[(i + len + 1) % len];
        }
    }



    RouteOptimizer::~RouteOptimizer()
    {
    }

    Route * RouteOptimizer::optimize(int passes)
    {

        //cout << "Metric Before Optimization = " << metric(nodes) << endl;
        aplyOptimizationPasses(passes);
        //cout << "Metric After Optimization = " << metric(nodes) << endl;

        return toRoute();
    }

    void RouteOptimizer::aplyOptimizationPasses(int passes)
    {
        // Do some optimization here...
        int len = nodes.size();

        // Original Metric.
        float score = metric(nodes);

        // Optimize through routeNode flipping.
        // O(n).
        for (int i = 0; i < len; i++)
        {
            attemptFlip(i);

            float new_score = metric(nodes);
            if (new_score > score + 2)
            {
                throw new std::runtime_error("A Deoptimization has Occured!");
            }
            score = new_score;

        }

        // Try swapping route pointers...
        // O(n^3)
        //for(int pass = 0; pass < 2; pass++)
        for (int pass = 0; pass < passes; pass++)
        {
            for (int i1 = 0; i1 < len; i1++)
            for (int i2 = i1 + 1; i2 < len; i2++)
            {
                attemptSwap(i1, i2);

                float new_score = metric(nodes);
                if (new_score > score + 2)
                {
                    throw new std::runtime_error("A Deoptimization has Occured!");
                }
                new_score = score;

            }

            //cout << "Metric After Pass " << pass << " = " << metric(nodes) << endl;
        }

        //cout << "Metric After Pass " << passes << " = " << metric(nodes) << endl;

        return;
    }

    Route * RouteOptimizer::toRoute()
    {
        Route * output = new Route();

        // Convert RouteOptimizer Structures back to a route representation.

        // retrive the start index, such that we start after the most distant edge.
        this -> start_index = getLongestEdgeIndex(nodes);

        RouteNode * first   = &nodes[this -> start_index];
        RouteNode * current = first;
        do
        {
            int ID = current -> id;
            Polyline * polyline = this -> route -> at(ID);
            
            // NOTE: We could easily make an output format that gives the users the flipped bools, instead of flipped lists.
            // I like the lists, because we could potentially chop up paths in the future for other applications.
            if (current -> flipped) 
            { 
                polyline = reverse_polyline(polyline);
            }
            else
            {
                polyline = copy_polyline(polyline);
            }

            output -> push_back(polyline);

            // Iterate.
            current = current -> next;
        }while(current != first);

        return output;
    }

    // -- Optimization Helper Functions.

    bool RouteOptimizer::attemptFlip(int i)
    {
        RouteNode & node = nodes[i];
        
        // The back connection.
        int ip0 = node.prev -> index_end;
        int ip1 = node.index_start;

        // The forward connection.
        int ip2 = node.index_end;
        int ip3 = node.next -> index_start;

        float metric_0 = metric(ip0, ip1) + metric(ip2, ip3);
        float metric_1 = metric(ip0, ip2) + metric(ip1, ip3);

        if(metric_1 < metric_0)
        {
            node.flip();
            return true;
        }

        return false;
    }

    // Attempts to point i1 to i2 and 
    bool RouteOptimizer::attemptSwap(int i1, int i2)
    {
        RouteNode * node1 = &nodes[i1];
        RouteNode * node2 = &nodes[i2];

        RouteNode * next1 = node1 -> next;
        RouteNode * next2 = node2 -> next;

        // Swaps are impossible for successive nodes.
        if(next1 == node2)
        {
            return false;
        }

        RouteNode * prev1 = node1 -> prev;
        RouteNode * prev2 = node2 -> prev;

        float metric_0 = metric(node1 -> index_end, next1 -> index_start) + 
                         metric(node2 -> index_end, next2 -> index_start);
        float metric_1 = metric(node1 -> index_end, node2 -> index_end) + 
                         metric(next1 -> index_start, next2 -> index_start);
        // No benifit.
        if (metric_0 < metric_1)
        {
            return false;
        }

        node1 -> next = node2;
        next1 -> prev = next2;// reversed later.
        next2 -> prev = next1;// Link the latter part back. This won't need to be reversed.
        node2 -> next = node1;// reversed later.

        // Reverse next1 through node2.

        //next1 -> node2 + .
        RouteNode * current = next1;
        RouteNode * end = node2;
        do
        {
            RouteNode * next = current -> next;
            current -> reverse();
            current = next;
        }while(current != end);
        current -> reverse();// Reverse the end.

        // Perform some opportunistic flips.
        attemptFlip(node1 -> id);
        attemptFlip(next1 -> id);
        attemptFlip(node2 -> id);
        attemptFlip(next2 -> id);
        
        return true;
    }

    // global metric.
    // Returns a consistent for how long the path is.
    // We wish to minimize this value.
    // We use this metric for open and closed problems, then simply remove the longest edge afterwards.
    float RouteOptimizer::metric(std::vector<RouteNode> nodes)
    {
        // Accumulator.
        float accum = 0.0f;
        // Warning, this is going in arbitrary original order, because the order that we add the segments up doesn't matter.
        // If you change this to rely on the permuted ordering of nodes, transverse this using next pointers.
        for (auto iter = nodes.begin(); iter != nodes.end(); ++iter)
        {
            RouteNode * node = &(*iter);

            // Point Indices.
            int p0 = node -> index_end;
            int p1 = node -> next -> index_start;

            accum += metric(p0, p1);
        }

        return accum;
    }

    int RouteOptimizer::getLongestEdgeIndex(std::vector<RouteNode> nodes)
    {
        int index = -1;
        float max = std::numeric_limits<float>::min();
        int len = nodes.size();
        for (int i = 0; i < len; i++)
        {
            RouteNode * node = &nodes[i];
            RouteNode * next = node -> next;

            // Point Indices.
            int p0 = node -> index_end;
            int p1 = next -> index_start;

            float dist = metric(p0, p1);

            if (dist > max)
            {
                index = next -> id;
                max = dist;
            }
        }

        if (index < 0)
        {
            throw new std::runtime_error("No maximum edge distance was found.");
        }

        return index;
    }

    // Returns a consistent heuristic for the length of a path from id1 to id2.
    float RouteOptimizer::metric(int id1, int id2)
    {
        ofPoint p1 = points[id1];
        ofPoint p2 = points[id2];
        
        // FIXME: Square distance would be faster.
        return ofDist(p1.x, p1.y, p2.x, p2.y);
    }

    void RouteOptimizer::permute(std::vector<int> & permutation_in_out)
    {
        
        std::vector<int> permutation;
        std::vector<int> permutation_out;

        // First compute the relative permutation from the input route to this optimizer to the present route.
        // ASSERTION( This loop goes n times, where n = nodes.length;
        RouteNode * start = &nodes[this -> start_index];
        RouteNode * current = start;
        do
        {
            permutation.push_back(current -> id);
            current = current -> next;
        } while (current != start);

        // Next permute the input permutation by the relative permutation.
        // i is the local index -> relative index -> global index.
        int len = permutation_in_out.size();
        for (int i = 0; i < len; i++)
        {
            int relative_index = permutation[i];
            int global_index   = permutation_in_out[relative_index];
            permutation_out.push_back(global_index);
        }

        // Finnally copy the resultant global permutation onto the input_output buffer.
        for (int i = 0; i < len; i++)
        {
            permutation_in_out[i] = permutation_out[i];
        }

        return;
    }

}