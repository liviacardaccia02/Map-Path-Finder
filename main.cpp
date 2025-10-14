#include "Graph.h"
#include <iostream>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }
    std::string filename = argv[1];
    try
    {
        Graph graph(filename);
        // Aggiungi codice per testare (es. stampa vicini di 86771)
        int testVertexId = 86771;
        const std::vector<Edge> &neighbors = graph.getNeighbors(testVertexId);
        std::cout << "Neighbors of vertex " << testVertexId << ":" << std::endl;
        for (const Edge &edge : neighbors)
        {
            std::cout << "Edge to " << edge.getEndId() << " with weight " << edge.getWeight() << std::endl;
        }
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}