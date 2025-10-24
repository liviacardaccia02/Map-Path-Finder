#ifndef UTILS_H
#define UTILS_H

#include <string_view>
#include "Graph.h"

namespace utils
{
    /**
     * @brief Extracts the next field from a comma-separated line.
     *
     * @param line A reference to the string_view representing the line to parse.
     * @return A string_view representing the next field.
     */
    std::string_view nextField(std::string_view &line);

    /**
     * @brief Calculate the medium point (average latitude and longitude) of all vertices in the graph.
     *
     * @param graph The graph containing the vertices.
     * @return A pair of (mediumLatitude, mediumLongitude).
     */
    std::pair<double, double> mediumPoint(const Graph &graph);

    /**
     * @brief Performs local Mercator projection for two vertices in the graph
     * returns the projected coordinates as a pair of ((x1, y1), (x2, y2)).
     *
     * @param graph The graph containing the vertices.
     * @param vertex1 The first vertex to project.
     * @param vertex2 The second vertex to project.
     * @return A pair of projected coordinates.
     */
    std::pair<std::pair<double, double>, std::pair<double, double>> mercatorProjection(const Graph &graph, const Vertex &vertex1, const Vertex &vertex2);

    /**
     * @brief Computes the Euclidean distance between two vertices using their projected coordinates.
     *
     * @param graph The graph containing the vertices.
     * @param v1 The first vertex.
     * @param v2 The second vertex.
     * @return The Euclidean distance in meters.
     */
    double computeEuclideanDistance(const Graph &graph, const Vertex &v1, const Vertex &v2);

    /**
     * Heuristic function for A* algorithm: estimates the cost from current to goal vertex
     * by using the Harvesine formula.
     * @brief Computes the Haversine distance between two vertices on the Earth's surface.
     *
     * @param current The current vertex.
     * @param goal The goal vertex.
     * @return The Haversine distance in meters.
     */
    double computeHaversineDistance(const Vertex &current, const Vertex &goal);
}

#endif