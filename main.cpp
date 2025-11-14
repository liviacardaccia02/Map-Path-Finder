#include <iostream>
#include "Graph.h"
#include "GraphicGraph.h"
#include "algorithms.h"
#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <memory>

void runAlgorithm(const std::string &algorithm, const Graph &graph, uint32_t startId, uint32_t endId)
{
    if (algorithm == "bfs")
    {
        algorithms::bfs(graph, startId, endId);
    }
    else if (algorithm == "dijkstra")
    {
        algorithms::dijkstra(graph, startId, endId);
    }
    else if (algorithm == "astar")
    {
        algorithms::aStar(graph, startId, endId);
    }
    else
    {
        std::cerr << "Error: Unknown algorithm '" << algorithm << "'. Use bfs, dijkstra, or astar." << std::endl;
    }
}

int main(int argc, char *argv[])
{
    std::string start = "", end = "";
    std::string algorithm;
    std::string filename;
    std::string mode;

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
        else if (arg == "--mode" && i + 1 < argc)
            mode = argv[++i];
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
    if (mode.empty())
    {
        mode = "text"; // default mode
    }
    else if (mode != "text" && mode != "graphic")
    {
        std::cerr << "Error: --mode must be 'text' or 'graphic'." << std::endl;
        return 1;
    }

    uint32_t startId = 0, endId = 0;
    try
    {
        startId = std::stoul(start);
        endId = std::stoul(end);
    }
    catch (const std::invalid_argument &)
    {
        std::cerr << "Error: --start and --end must be numeric vertex IDs." << std::endl;
        return 1;
    }
    catch (const std::out_of_range &)
    {
        std::cerr << "Error: --start or --end value out of range." << std::endl;
        return 1;
    }

    if (mode == "text")
    {
        try
        {
            Graph graph(filename);
            runAlgorithm(algorithm, graph, startId, endId);
            return 0;
        }
        catch (const std::runtime_error &e)
        {
            std::cerr << e.what() << std::endl;
            return 1;
        }
    }
    else if (mode == "graphic")
    {
        QApplication app(argc, argv);

        // create the scene before constructing GraphicGraph (it expects a valid scene pointer)
        auto scene = std::make_unique<QGraphicsScene>(-500, -500, 1000, 1000);

        try
        {
            GraphicGraph graph(filename, scene.get());
            runAlgorithm(algorithm, graph, startId, endId);

            QGraphicsView view(scene.get());
            view.show();
            return app.exec();
        }
        catch (const std::runtime_error &e)
        {
            std::cerr << e.what() << std::endl;
            return 1;
        }
    }
    else
    {
        std::cerr << "Error: Unknown mode '" << mode << "'. Use 'text' or 'graphic'." << std::endl;
        return 1;
    }
}