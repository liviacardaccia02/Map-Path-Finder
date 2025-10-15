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
            int id;
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
            int idStart, idEnd;
            double weight;

            idStart = std::stoi(std::string(utils::nextField(sv)));
            idEnd = std::stoi(std::string(utils::nextField(sv)));
            weight = std::stod(std::string(utils::nextField(sv)));

            Edge e(idStart, idEnd, weight);
            addEdge(e);
            edgeCount++;
        }
    }

    std::cout << "Loaded " << vertexCount << " vertices and " << edgeCount << " edges.\n";
    file.close();
}

void Graph::addVertex(const Vertex &vertex)
{
    vertices.insert_or_assign(vertex.getId(), vertex);
}

void Graph::addEdge(const Edge &edge)
{
    adjacencyList[edge.getStartId()].push_back(edge);
}

const std::vector<Edge> &Graph::getNeighbors(int vertexId) const
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