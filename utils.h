#ifndef UTILS_H
#define UTILS_H

#include <string_view>
#include "Graph.h"

namespace utils
{
    /* Extract the next field from a comma-separated line */
    std::string_view nextField(std::string_view &line);

    /* Calculate the medium point (average latitude and longitude) of all vertices in the graph
     * returns a pair of (mediumLatitude, mediumLongitude)
     */
    std::pair<double, double> mediumPoint(const Graph &graph);

    /* Perform local Mercator projection for two vertices in the graph
     *  returns the projected coordinates as a pair of ((x1, y1), (x2, y2))
     */
    std::pair<std::pair<double, double>, std::pair<double, double>> mercatorProjection(const Graph &graph, const Vertex &vertex1, const Vertex &vertex2);

    /* Compute the Euclidean distance between two vertices */
    double computeEuclideanDistance(const Graph &graph, const Vertex &v1, const Vertex &v2);
}

#endif