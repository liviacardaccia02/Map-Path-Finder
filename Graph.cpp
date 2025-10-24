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
    initializeFromFile(filename);
}

void Graph::initializeFromFile(const std::string &filename)
{
    std::cout << "Creating Graph from file: " << filename << "\n";
    std::ifstream file(filename);
    if (!file.is_open())
    {
        throw std::runtime_error("Error: could not open file " + filename);
    }

    int vertexCount = 0;
    int edgeCount = 0;
    std::string line;
    int lineNumber = 0;

    while (std::getline(file, line))
    {
        lineNumber++;

        if (line.empty() || line[0] == '#')
            continue; // skip empty lines or comments

        std::string_view sv(line);
        char type = sv.front();
        sv.remove_prefix(2); // remove "V," or "E,"

        if (type == 'V')
        {
            uint32_t id;
            double longitude, latitude;

            try
            {
                id = std::stoi(std::string(utils::nextField(sv)));
                longitude = std::stod(std::string(utils::nextField(sv)));
                latitude = std::stod(std::string(utils::nextField(sv)));
            }
            catch (const std::invalid_argument &e)
            {
                throw std::runtime_error("Parsing error at line " + std::to_string(lineNumber) +
                                         ": Invalid number format in vertex definition (possibly due to comments or malformed data).\n" +
                                         "Line content: " + line + "\n" +
                                         "Hint: Check for inline comments (//) or non-numeric characters.");
            }
            catch (const std::out_of_range &e)
            {
                throw std::runtime_error("Parsing error at line " + std::to_string(lineNumber) +
                                         ": Number out of range in vertex definition.\n" +
                                         "Line content: " + line);
            }

            Vertex v(id, longitude, latitude);
            addVertex(v);
            vertexCount++;
        }
        else if (type == 'E')
        {
            std::string parsedWeight;
            uint32_t idStart, idEnd;
            double weight;

            try
            {
                idStart = std::stoi(std::string(utils::nextField(sv)));
                idEnd = std::stoi(std::string(utils::nextField(sv)));
                parsedWeight = std::string(utils::nextField(sv));
                weight = parsedWeight.empty() || parsedWeight == "0" ? utils::computeEuclideanDistance(*this, getVertex(idStart), getVertex(idEnd))
                                                                     : std::stod(parsedWeight);
            }
            catch (const std::invalid_argument &e)
            {
                throw std::runtime_error("Parsing error at line " + std::to_string(lineNumber) +
                                         ": Invalid number format in edge definition (possibly due to comments or malformed data).\n" +
                                         "Line content: " + line + "\n" +
                                         "Hint: Check for inline comments (//) or non-numeric characters.");
            }
            catch (const std::out_of_range &e)
            {
                throw std::runtime_error("Parsing error at line " + std::to_string(lineNumber) +
                                         ": Number out of range in edge definition.\n" +
                                         "Line content: " + line);
            }

            Edge e(idStart, idEnd, weight);
            addEdge(e);
            edgeCount++;
        }
    }

    file.close();
}

void Graph::addVertex(const Vertex &vertex)
{
    // std::cout << "Adding vertex " << vertex.getId() << " to Graph\n";
    vertices.insert_or_assign(vertex.getId(), vertex);
}

void Graph::addEdge(const Edge &edge)
{
    // std::cout << "Adding edge " << edge.getStartId() << " -> " << edge.getEndId() << " to Graph\n";
    uint32_t startId = edge.getStartId();
    uint32_t endId = edge.getEndId();
    if (vertices.find(startId) == vertices.end() || vertices.find(endId) == vertices.end())
    {
        throw std::runtime_error("Edge connects to non-existent vertex");
    }
    adjacencyList[startId].push_back(edge);
}

const std::unordered_map<uint32_t, Vertex> &Graph::getVertices() const
{
    return vertices;
}

const std::unordered_map<uint32_t, std::vector<Edge>> &Graph::getAdjacencyList() const
{
    return adjacencyList;
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

void Graph::drawPath(const std::vector<uint32_t> &path) const
{
    std::cout << "Drawing path in Graph (no graphical representation available)\n";
}
