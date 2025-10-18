#include <iostream>
#include "Graph.h"
#include "algorithms.h"

int main(int argc, char *argv[])
{
    std::string start = "", end = "";
    std::string algorithm;
    std::string filename;

    // Argument parsing
    for (int i = 1; i < argc; i++)
    {
        std::string arg = argv[i];
        if (arg == "--start" && i + 1 < argc)
            start = argv[++i];
        else if (arg == "--end" && i + 1 < argc)
            end = argv[++i];
        else if (arg == "--algorithm" && i + 1 < argc)
            algorithm = argv[++i];
        else if (arg == "--file" && i + 1 < argc)
            filename = argv[++i];
    }

    // Input validation
    if (start == "" || end == "")
    {
        std::cerr << "Error: --start and --end are required." << std::endl;
        return 1;
    }
    if (algorithm.empty())
    {
        std::cerr << "Error: --algorithm is required. Available options are: bfs, dijkstra, astar." << std::endl;
        return 1;
    }
    if (filename.empty())
    {
        std::cerr << "Error: --file is required. Please specify the graph file." << std::endl;
        return 1;
    }

    try
    {
        Graph graph(filename);

        if (algorithm == "bfs")
        {
            algorithms::bfs(graph, std::stoul(start), std::stoul(end));
        }
        else if (algorithm == "dijkstra")
        {
            std::cout << "Dijkstra not implemented yet." << std::endl;
        }
        else if (algorithm == "astar")
        {
            std::cout << "A* not implemented yet." << std::endl;
        }
        else
        {
            std::cerr << "Error: Unknown algorithm '" << algorithm << "'. Use bfs, dijkstra, or astar." << std::endl;
            return 1;
        }

        return 0;
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}