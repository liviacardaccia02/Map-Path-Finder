#include "utils.h"
#include <cmath>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

using namespace utils;

std::string_view utils::nextField(std::string_view &line)
{
    size_t pos = line.find(',');
    std::string_view field = line.substr(0, pos);
    if (pos == std::string_view::npos)
        line = {};
    else
        line.remove_prefix(pos + 1);
    return field;
}

std::pair<double, double> utils::mediumPoint(const Graph &graph)
{
    double sumLat, sumLong = 0.0;
    size_t count = 0;

    for (const auto &pair : graph.getVertices())
    {
        sumLat += pair.second.getLatitude();
        sumLong += pair.second.getLongitude();
        count++;
    }

    return count > 0 ? std::make_pair(sumLat / count, sumLong / count) : std::make_pair(0.0, 0.0);
}

std::pair<std::pair<double, double>, std::pair<double, double>> utils::mercatorProjection(const Graph &graph, const Vertex &v1, const Vertex &v2)
{
    int scale = 111000; // Approximate scale factor for converting degrees to meters
    double midLatitude = utils::mediumPoint(graph).first;
    double midLongitude = utils::mediumPoint(graph).second;

    double x1 = scale * (v1.getLongitude() - midLongitude) * cos(midLatitude * M_PI / 180.0);
    double y1 = scale * (v1.getLatitude() - midLatitude) * cos(midLatitude * M_PI / 180.0);
    double x2 = scale * (v2.getLongitude() - midLongitude) * cos(midLatitude * M_PI / 180.0);
    double y2 = scale * (v2.getLatitude() - midLatitude) * cos(midLatitude * M_PI / 180.0);

    return std::pair<std::pair<double, double>, std::pair<double, double>>(std::make_pair(x1, y1), std::make_pair(x2, y2));
}

double utils::computeEuclideanDistance(const Graph &graph, const Vertex &v1, const Vertex &v2)
{
    auto [proj1, proj2] = mercatorProjection(graph, v1, v2);
    double dx = proj1.first - proj2.first;
    double dy = proj1.second - proj2.second;
    return std::sqrt(dx * dx + dy * dy);
}
