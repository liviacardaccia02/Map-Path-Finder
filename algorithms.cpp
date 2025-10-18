#include "algorithms.h"
#include <iomanip>
#include <algorithm>

void algorithms::bfs(const Graph &graph, uint32_t startVertexId, uint32_t endVertexId)
{
    if (startVertexId == endVertexId)
    {
        std::cout << "Start and end vertices are the same." << std::endl;
        return;
    }
    else if (graph.getVertices().find(startVertexId) == graph.getVertices().end() || graph.getVertices().find(endVertexId) == graph.getVertices().end())
    {
        std::cout << "Start or end vertex not found in the graph." << std::endl;
        return;
    }
    else
    {
        std::queue<uint32_t> queue;                      // queue of vertex IDs to visit
        std::unordered_set<uint32_t> visited;            // set of visited vertex IDs
        std::unordered_map<uint32_t, uint32_t> parent;   // to reconstruct the path
        std::unordered_map<uint32_t, uint32_t> distance; // to track cumulative distances

        queue.push(startVertexId);
        visited.insert(startVertexId);
        parent[startVertexId] = -1; // no parent for start vertex
        distance[startVertexId] = 0.0;

        int visitedCount = 0;

        while (!queue.empty())
        {
            int current = queue.front();
            queue.pop();
            visitedCount++;

            const std::vector<Edge> &neighbors = graph.getNeighbors(current);
            for (const Edge &edge : neighbors)
            {
                uint32_t neighbor = edge.getEndId();
                double weight = edge.getWeight();

                if (visited.find(neighbor) == visited.end())
                {
                    queue.push(neighbor);
                    visited.insert(neighbor);
                    parent[neighbor] = current;
                    distance[neighbor] = distance[current] + weight;
                }

                if (neighbor == endVertexId)
                {
                    std::vector<std::pair<uint32_t, double>> path;
                    int node = endVertexId;
                    while (node != -1)
                    {
                        path.push_back({node, distance[node]});
                        node = parent[node]; // predecessor
                    }
                    std::reverse(path.begin(), path.end()); // from start to end

                    std::cout << "Total visited vertex = " << visitedCount << std::endl;

                    // functions std::setw(), std::fixed and std::setprecision() are used to format the output
                    std::cout << "Total vertex on path from start to end = " << path.size() << std::endl;
                    for (size_t i = 0; i < path.size(); ++i)
                    {
                        std::cout << "Vertex[" << std::setw(4) << (i + 1) << "] = " << std::setw(8) << path[i].first
                                  << ", length = " << std::fixed << std::setprecision(2) << path[i].second << std::endl;
                    }
                    return;
                }
            }
        }
        std::cout << "No path found from vertex " << startVertexId << " to vertex " << endVertexId << ".\n"
                  << std::endl;
    }
}

void algorithms::dijkstra(const Graph &graph, uint32_t startVertexId)
{
    return;
}

void algorithms::aStar(const Graph &graph, uint32_t startVertexId, uint32_t goalVertexId)
{
    return;
}
