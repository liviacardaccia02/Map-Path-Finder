#include "Graph.h"
#include "Vertex.h"
#include "Edge.h"
#include "utils.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>

Graph::Graph(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        throw std::runtime_error("Error: could not open file " + filename);
    }

    int vertexCount = 0;
    int edgeCount = 0;
    std::string line;

    while (std::getline(file, line))
    {
        if (line.empty() || line[0] == '#')
            continue; // skip empty lines or comments

        std::string_view sv(line);
        char type = sv.front();
        sv.remove_prefix(2); // remove "V," or "E,"

        if (type == 'V')
        {
            uint32_t id;
            double longitude, latitude;

            id = std::stoi(std::string(utils::nextField(sv)));
            longitude = std::stod(std::string(utils::nextField(sv)));
            latitude = std::stod(std::string(utils::nextField(sv)));

            Vertex v(id, longitude, latitude);
            addVertex(v);
            vertexCount++;
        }
        else if (type == 'E')
        {
            std::string parsedWeight;
            uint32_t idStart, idEnd;
            double weight;

            idStart = std::stoi(std::string(utils::nextField(sv)));
            idEnd = std::stoi(std::string(utils::nextField(sv)));
            parsedWeight = std::string(utils::nextField(sv));
            weight = parsedWeight.empty() ? utils::computeEuclideanDistance(*this, getVertex(idStart), getVertex(idEnd))
                                          : std::stod(parsedWeight);
            std::cout << "Computed weight for edge " << idStart << "->" << idEnd << " as " << weight << std::endl;
            Edge e(idStart, idEnd, weight);
            addEdge(e);
            edgeCount++;
        }
    }

    file.close();
}

void Graph::addVertex(const Vertex &vertex)
{
    vertices.insert_or_assign(vertex.getId(), vertex);
}

void Graph::addEdge(const Edge &edge)
{
    uint32_t startId = edge.getStartId();
    uint32_t endId = edge.getEndId();
    if (vertices.find(startId) == vertices.end() || vertices.find(endId) == vertices.end())
    {
        throw std::runtime_error("Edge connects to non-existent vertex"); // TODO better error handling
    }
    adjacencyList[startId].push_back(edge);
}

const std::unordered_map<uint32_t, Vertex> &Graph::getVertices() const
{
    return vertices;
}

const std::vector<Edge> &Graph::getNeighbors(uint32_t vertexId) const
{
    static const std::vector<Edge> kEmpty;
    auto it = adjacencyList.find(vertexId);
    if (it != adjacencyList.end())
    {
        return it->second;
    }
    else
    {
        return kEmpty;
    }
}

Vertex Graph::getVertex(uint32_t vertexId) const
{
    auto it = vertices.find(vertexId);
    if (it != vertices.end())
    {
        return it->second;
    }
    throw std::runtime_error("Vertex not found");
}
