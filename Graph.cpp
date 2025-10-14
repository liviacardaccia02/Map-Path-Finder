#include "Graph.h"
#include "Vertex.h"
#include "Edge.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>

Graph::Graph(const std::string &filename)
{
    std::ifstream file(filename); // Apri il file
    if (!file.is_open())
    {
        throw std::runtime_error("Error: could not open file " + filename);
    }

    int vertexCount = 0; // TODO rimuovere
    int edgeCount = 0;   // TODO rimuovere

    std::string line;
    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        std::string token;

        if (std::getline(ss, token, ','))
        {
            std::cout << "Token: " << token << std::endl; // TODO rimuovere
            if (token == "V")                             // Vertex
            {
                int id;
                double longitude, latitude;
                if (ss >> id >> longitude >> latitude)
                {
                    Vertex v(id, latitude, longitude);
                    addVertex(v);
                    vertexCount++; // TODO rimuovere
                    std::cout << "Aggiunto vertice " << id << std::endl;
                }
                else
                {
                    std::cout << "Parsing vertice fallito per linea: " << line << std::endl;
                }
            }
            else if (token == "E") // Edge
            {
                int fromId, toId;
                double length;
                std::string rest;
                if (std::getline(ss, rest))
                { // Leggi il resto della riga
                    std::stringstream ssRest(rest);
                    if (ssRest >> fromId >> toId >> length)
                    {
                        std::string name;
                        std::getline(ssRest, name); // Ignora il nome se presente
                        Edge e(fromId, toId, length);
                        addEdge(e);
                        edgeCount++;
                        std::cout << "Aggiunto edge da " << fromId << " a " << toId << std::endl;
                    }
                    else
                    {
                        // std::cout << "Parsing edge fallito per linea: " << line << std::endl;
                    }
                }
            }
        }
    }

    // TODO rimuovere
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
