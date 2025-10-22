#include "algorithms.h"
#include "utils.h"
#include <iomanip>
#include <algorithm>
#include <queue>
#include <set>
#include <limits>
#include <chrono>

void algorithms::bfs(const Graph &graph, uint32_t startVertexId, uint32_t endVertexId)
{
    if (startVertexId == endVertexId)
    {
        std::cout << "Start and end vertices are the same." << std::endl;
        return;
    }
    else if (graph.getNeighbors(startVertexId).empty() || graph.getNeighbors(endVertexId).empty())
    {
        std::cout << "Start or end vertex has no neighbors in the graph." << std::endl;
        return;
    }
    else if (graph.getVertices().find(startVertexId) == graph.getVertices().end() || graph.getVertices().find(endVertexId) == graph.getVertices().end())
    {
        std::cout << "Start or end vertex not found in the graph." << std::endl;
        return;
    }
    else
    {
        auto start = std::chrono::steady_clock::now();

        std::queue<uint32_t> queue;                    // Queue of vertex IDs to visit
        std::unordered_set<uint32_t> visited;          // Set of visited vertex IDs
        std::unordered_map<uint32_t, uint32_t> parent; // To reconstruct the path
        std::unordered_map<uint32_t, double> distance; // To track cumulative distances

        queue.push(startVertexId);
        visited.insert(startVertexId);

        // Initialize parent and distance for the start vertex - std::numeric_limits<uint32_t>::max() is used since the vertices ID are uint32_t
        parent[startVertexId] = std::numeric_limits<uint32_t>::max(); // No parent for start vertex
        distance[startVertexId] = 0.0;

        int visitedCount = 0;

        while (!queue.empty())
        {
            uint32_t current = queue.front();
            queue.pop();
            visitedCount++;

            if (current == endVertexId)
            {
                // Reconstruct the path from end to start using the parent map
                std::vector<std::pair<uint32_t, double>> path;
                uint32_t step = endVertexId;
                while (step != std::numeric_limits<uint32_t>::max())
                {
                    path.push_back({step, distance[step]});
                    step = parent[step];
                }
                std::reverse(path.begin(), path.end());

                // Output formatting
                std::cout << "Total visited vertex = " << visitedCount << std::endl;
                std::cout << "Total vertices on path from start to end = " << path.size() << std::endl;
                for (size_t i = 0; i < path.size(); ++i)
                {
                    std::cout << "Vertex[" << std::setw(4) << (i + 1) << "] : id = " << std::setw(8) << path[i].first
                              << ", length = " << std::fixed << std::setprecision(2) << path[i].second << std::endl;
                }

                auto end = std::chrono::steady_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
                std::string timeStr = std::to_string(duration);
                if (timeStr.length() > 3)
                {
                    timeStr.insert(timeStr.length() - 3, ",");
                }
                std::cout << "INFO: path calculated in " << timeStr << "us" << std::endl;

                return;
            }

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
            }
        }
        std::cout << "Total visited vertex = " << visitedCount << std::endl;
        std::cout << "No path found from vertex " << startVertexId << " to vertex " << endVertexId << ".\n"
                  << std::endl;
        return;
    }
}

void algorithms::dijkstra(const Graph &graph, uint32_t startVertexId, uint32_t endVertexId)
{
    if (startVertexId == endVertexId)
    {
        std::cout << "Start and end vertices are the same." << std::endl;
        return;
    }
    else if (graph.getNeighbors(startVertexId).empty() || graph.getNeighbors(endVertexId).empty())
    {
        std::cout << "Start or end vertex has no neighbors in the graph." << std::endl;
        return;
    }
    else if (graph.getVertices().find(startVertexId) == graph.getVertices().end() || graph.getVertices().find(endVertexId) == graph.getVertices().end())
    {
        std::cout << "Start or end vertex not found in the graph." << std::endl;
        return;
    }
    else
    {
        auto start = std::chrono::steady_clock::now();

        int visitedCount = 0;
        std::unordered_set<uint32_t> visited;            // Set of visited vertex IDs
        std::unordered_map<uint32_t, uint32_t> previous; // To reconstruct the path
        std::unordered_map<uint32_t, double> distance;   // To track cumulative distances
        std::set<std::pair<double, uint32_t>> pq;        // Min-heap priority queue {distance, vertexId}

        for (const auto &pair : graph.getVertices())
        {
            distance[pair.first] = std::numeric_limits<double>::infinity(); // All vertices initially at infinite distance
        }

        distance[startVertexId] = 0.0;                                  // Distance to start vertex is 0
        previous[startVertexId] = std::numeric_limits<uint32_t>::max(); // No previous vertex for start vertex

        pq.insert({0.0, startVertexId});

        while (!pq.empty())
        {
            uint32_t currentVertexId = pq.begin()->second;
            pq.erase(pq.begin());

            if (currentVertexId == endVertexId)
            {
                break; // Found the shortest path to the end vertex
            }

            if (visited.find(currentVertexId) != visited.end())
            {
                continue; // Already visited
            }
            visited.insert(currentVertexId);
            visitedCount++;

            const std::vector<Edge> &neighbors = graph.getNeighbors(currentVertexId);
            for (const Edge &edge : neighbors)
            {
                uint32_t neighbor = edge.getEndId();
                double weight = edge.getWeight() < 0 ? throw std::runtime_error("Negative edge weight detected") : edge.getWeight(); // Stops if it finds negative weights
                double updatedDistance = distance[currentVertexId] + weight;

                if (updatedDistance < distance[neighbor])
                {
                    // Removes the old distance if it exists
                    if (distance[neighbor] != std::numeric_limits<double>::infinity())
                    {
                        pq.erase({distance[neighbor], neighbor});
                    }
                    distance[neighbor] = updatedDistance;
                    previous[neighbor] = currentVertexId;
                    // Only add to the priority queue if not visited
                    if (visited.find(neighbor) == visited.end())
                    {
                        pq.insert({updatedDistance, neighbor});
                    }
                }
            }
        }

        // Reconstruct the shortest path from endVertexId to startVertexId using the 'previous' map.
        double shortestDistance = distance[endVertexId];
        std::vector<std::pair<uint32_t, double>> path;
        if (shortestDistance == std::numeric_limits<double>::infinity())
        {
            // No path found if the shortest distance is infinity.
            path.clear();
            std::cout << "No path found from vertex " << startVertexId << " to vertex " << endVertexId << ".\n"
                      << std::endl;
            return;
        }
        else
        {
            // Build the path by following the 'previous' map from endVertexId to startVertexId.
            path.clear();
            uint32_t u = endVertexId;
            while (u != std::numeric_limits<uint32_t>::max())
            {
                path.push_back({u, distance[u]});
                u = previous[u];
            }
            std::reverse(path.begin(), path.end()); // Reverse to get path from start to end.

            // Output formatting: show visited count, path size, and each vertex with its cumulative distance.
            std::cout << "Total visited vertex = " << visitedCount << std::endl;
            std::cout << "Total vertices on path from start to end = " << path.size() << std::endl;
            for (size_t i = 0; i < path.size(); ++i)
            {
                std::cout << "Vertex[" << std::setw(4) << (i + 1) << "] : id = " << std::setw(8) << path[i].first
                          << ", length = " << std::fixed << std::setprecision(2) << path[i].second << std::endl;
            }

            auto end = std::chrono::steady_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
            std::string timeStr = std::to_string(duration);
            if (timeStr.length() > 3)
            {
                timeStr.insert(timeStr.length() - 3, ",");
            }
            std::cout << "INFO: path calculated in " << timeStr << "us" << std::endl;

            return;
        }
    }
}

double heuristic(const Vertex &current, const Vertex &goal)
{
    return utils::haversineDistance(current, goal);
}

void algorithms::aStar(const Graph &graph, uint32_t startVertexId, uint32_t goalVertexId)
{
    if (startVertexId == goalVertexId)
    {
        std::cout << "Start and goal vertices are the same." << std::endl;
        return;
    }
    else if (graph.getNeighbors(startVertexId).empty() || graph.getNeighbors(goalVertexId).empty())
    {
        std::cout << "Start or goal vertex has no neighbors in the graph." << std::endl;
        return;
    }
    else if (graph.getVertices().find(startVertexId) == graph.getVertices().end() || graph.getVertices().find(goalVertexId) == graph.getVertices().end())
    {
        std::cout << "Start or goal vertex not found in the graph." << std::endl;
        return;
    }
    else
    {
        auto start = std::chrono::steady_clock::now();

        int visitedCount = 0;
        std::unordered_set<uint32_t> visited;            // Set of visited vertex IDs
        std::unordered_map<uint32_t, uint32_t> previous; // To reconstruct the path
        std::unordered_map<uint32_t, double> distance;   // To track cumulative distances - g(n)
        std::set<std::pair<double, uint32_t>> pq;        // Min-heap priority queue {f(n), vertexId}

        const Vertex &goalVertex = graph.getVertex(goalVertexId);

        for (const auto &pair : graph.getVertices())
        {
            uint32_t vertexId = pair.first;
            distance[vertexId] = std::numeric_limits<double>::infinity();
        }

        distance[startVertexId] = 0.0;
        previous[startVertexId] = std::numeric_limits<uint32_t>::max();
        pq.insert({heuristic(graph.getVertex(startVertexId), goalVertex), startVertexId});

        while (!pq.empty())
        {
            uint32_t currentVertexId = pq.begin()->second;
            pq.erase(pq.begin());

            if (visited.find(currentVertexId) != visited.end())
                continue;
            visited.insert(currentVertexId);
            visitedCount++;

            if (currentVertexId == goalVertexId)
                break;

            const std::vector<Edge> &neighbors = graph.getNeighbors(currentVertexId);
            for (const Edge &edge : neighbors)
            {
                uint32_t neighbor = edge.getEndId();
                double weight = edge.getWeight();
                double g = distance[currentVertexId] + weight;

                if (g < distance[neighbor])
                {
                    if (distance[neighbor] != std::numeric_limits<double>::infinity())
                        pq.erase({distance[neighbor] + heuristic(graph.getVertex(neighbor), goalVertex), neighbor});
                    distance[neighbor] = g;
                    previous[neighbor] = currentVertexId;
                    double f = g + heuristic(graph.getVertex(neighbor), goalVertex);
                    pq.insert({f, neighbor});
                }
            }
        }

        double shortestDistance = distance[goalVertexId];
        std::vector<uint32_t> path;
        if (shortestDistance == std::numeric_limits<double>::infinity())
        {
            std::cout << "No path found from vertex " << startVertexId << " to vertex " << goalVertexId << ".\n"
                      << std::endl;
            return;
        }
        else
        {
            uint32_t u = goalVertexId;
            while (u != std::numeric_limits<uint32_t>::max())
            {
                path.push_back(u);
                u = previous[u];
            }
            std::reverse(path.begin(), path.end());

            std::cout << "Total visited vertex = " << visitedCount << std::endl;
            std::cout << "Total vertices on path from start to end = " << path.size() << std::endl;
            for (size_t i = 0; i < path.size(); ++i)
            {
                std::cout << "Vertex[" << std::setw(4) << (i + 1) << "] : id = " << std::setw(8) << path[i]
                          << ", length = " << std::fixed << std::setprecision(2) << distance[path[i]] << std::endl;
            }

            auto end = std::chrono::steady_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
            std::string timeStr = std::to_string(duration);
            if (timeStr.length() > 3)
            {
                timeStr.insert(timeStr.length() - 3, ",");
            }
            std::cout << "INFO: path calculated in " << timeStr << "us" << std::endl;

            return;
        }
    }
}