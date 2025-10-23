#ifndef VERTEX_H
#define VERTEX_H

#include <cstdint>

class Vertex
{
private:
    uint32_t id;
    double longitude, latitude;

public:
    /* Constructor & destructor */
    Vertex(uint32_t id, double longitude, double latitude) : id(id), longitude(longitude), latitude(latitude) {}
    ~Vertex() = default;

    /* Getters */
    uint32_t getId() const { return id; }
    double getLongitude() const { return longitude; }
    double getLatitude() const { return latitude; }
};

#endif