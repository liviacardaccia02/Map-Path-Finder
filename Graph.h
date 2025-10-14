#ifndef GRAPH_H
#define GRAPH_H

#include <unordered_map>
#include <vector>
#include "Vertex.h"
#include "Edge.h"

class Graph
{
private:
    std::unordered_map<int, Vertex> vertices;
    std::unordered_map<int, std::vector<Edge>> adjacencyList;

public:
    Graph() = default;
    ~Graph() = default;

    /*
        Adds a vertex to the graph.
    */
    void addVertex(const Vertex &vertex);

    /*
        Adds an edge to the graph.
    */
    void addEdge(const Edge &edge);

    /*
        Returns a list of edges (neighbors) connected to the given vertex ID.
    */
    std::vector<Edge> getNeighbors(int vertexId) const;
};

#endif