#include "Graph.h"

void Graph::addVertex(const Vertex &vertex)
{
    vertices[vertex.getId()] = vertex;
}

void Graph::addEdge(const Edge &edge)
{
    adjacencyList[edge.getStart().getId()].push_back(edge);
}

std::vector<Edge> Graph::getNeighbors(int vertexId) const
{
    std::vector<Edge> neighbors;
    auto it = adjacencyList.find(vertexId);
    if (it != adjacencyList.end())
    {
        neighbors = it->second;
    }
    return neighbors;
}
