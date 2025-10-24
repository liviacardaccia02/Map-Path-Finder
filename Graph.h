#ifndef GRAPH_H
#define GRAPH_H

#include <unordered_map>
#include <vector>
#include <string>
#include <cstdint>
#include "Vertex.h"
#include "Edge.h"

class Graph
{
private:
    std::unordered_map<uint32_t, Vertex> vertices;
    std::unordered_map<uint32_t, std::vector<Edge>> adjacencyList;

public:
    /*
        Constructor that initializes the graph from a given file name.
        The file is expected to contain vertex and edge definitions.
    */
    Graph(const std::string &filename);

    /**
     * Helper method to initialize the graph from a file.
     * @param filename The name of the file containing the graph data.
     */
    virtual void initializeFromFile(const std::string &filename);

    /**
     * Virtual method to add a vertex to the graph.
     * This method can be overridden in derived classes for additional functionality.
     * @param vertex The vertex to be added.
     */
    virtual void addVertex(const Vertex &vertex);

    /**
     * Virtual method to add an edge to the graph.
     * This method can be overridden in derived classes for additional functionality.
     * @param edge The edge to be added.
     */
    virtual void addEdge(const Edge &edge);

    /*
        Returns the map of vertices in the graph.
    */
    const std::unordered_map<uint32_t, Vertex> &getVertices() const;

    /*
        Returns the adjacency list of the graph.
    */
    const std::unordered_map<uint32_t, std::vector<Edge>> &getAdjacencyList() const;

    /*
        Returns the list of edges (neighbors) connected to the given vertex ID.
        Note: returns a const reference to avoid copying potentially large neighbor lists.
    */
    const std::vector<Edge> &getNeighbors(uint32_t vertexId) const;

    /*
        Returns the vertex corresponding to the given vertex ID.
        Throws an exception if the vertex does not exist.
    */
    Vertex getVertex(uint32_t vertexId) const;

    /*
        Virtual method to draw the path on the graph.
        This method can be overridden in derived classes for graphical representation.
    */
    virtual void drawPath(const std::vector<uint32_t> &path) const;

    /*
        Virtual destructor.
    */
    virtual ~Graph() = default;
};

#endif