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
    double midLatitude = utils::mediumPoint(graph).first;
    double midLongitude = utils::mediumPoint(graph).second;

    // Approximate scale factors for converting degrees to meters
    int scale_y = 111000 * 0.88;
    int scale_x = scale_y * cos(midLatitude * M_PI / 180);

    double x1 = scale_x * (v1.getLongitude() - midLongitude);
    double y1 = scale_y * (v1.getLatitude() - midLatitude);
    double x2 = scale_x * (v2.getLongitude() - midLongitude);
    double y2 = scale_y * (v2.getLatitude() - midLatitude);

    return std::pair<std::pair<double, double>, std::pair<double, double>>(std::make_pair(x1, y1), std::make_pair(x2, y2));
}

double utils::computeEuclideanDistance(const Graph &graph, const Vertex &v1, const Vertex &v2)
{
    auto [proj1, proj2] = mercatorProjection(graph, v1, v2);
    double dx = proj1.first - proj2.first;
    double dy = proj1.second - proj2.second;
    return std::sqrt(dx * dx + dy * dy);
}

double utils::haversineDistance(const Vertex &v1, const Vertex &v2)
{
    double radius = 6371000.0; // Radius of the Earth in meters

    // Convert degrees to radians
    double lat1 = v1.getLatitude() * M_PI / 180.0;
    double lon1 = v1.getLongitude() * M_PI / 180.0;
    double lat2 = v2.getLatitude() * M_PI / 180.0;
    double lon2 = v2.getLongitude() * M_PI / 180.0;

    double dLat = lat2 - lat1;
    double dLon = lon2 - lon1;

    double a = sin(dLat / 2) * sin(dLat / 2) +
               cos(lat1) * cos(lat2) * sin(dLon / 2) * sin(dLon / 2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));
    double distance = radius * c;

    return distance;
}
