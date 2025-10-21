#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "Graph.h"
#include <iostream>
#include <queue>
#include <unordered_set>

namespace algorithms
{
    /* Performs a breadth-first search (BFS) on the graph. */
    void bfs(const Graph &graph, uint32_t startVertexId, uint32_t endVertexId);

    /* Performs Dijkstra's algorithm on the graph,
     * finding the shortest path from the start vertex to all other vertices
     * and assuming that there are no negative weights on the edges. */
    void dijkstra(const Graph &graph, uint32_t startVertexId, uint32_t endVertexId);

    /* Performs the A* search algorithm on the graph,
     * finding the shortest path from the start vertex to the goal vertex
     * using a heuristic to guide the search. */
    void aStar(const Graph &graph, uint32_t startVertexId, uint32_t goalVertexId);
};

#endif