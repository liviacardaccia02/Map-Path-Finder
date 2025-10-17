#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "Graph.h"

namespace algorithms
{
    /* Performs a breadth-first search (BFS) on the graph. */
    Graph bfs(const Graph &graph, int startVertexId);

    /* Performs Dijkstra's algorithm on the graph,
     * finding the shortest path from the start vertex to all other vertices
     * and assuming that there are no negative weights on the edges. */
    Graph dijkstra(const Graph &graph, int startVertexId);

    /* Performs the A* search algorithm on the graph,
     * finding the shortest path from the start vertex to the goal vertex
     * using a heuristic to guide the search. */
    Graph aStar(const Graph &graph, int startVertexId, int goalVertexId);
};

#endif