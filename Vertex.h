#ifndef VERTEX_H
#define VERTEX_H

class Vertex
{
private:
    int id;
    double longitude, latitude;

public:
    /* Constructor */
    Vertex(int id, double longitude, double latitude) : id(id), longitude(longitude), latitude(latitude) {}
    ~Vertex() = default;

    /* Getters */
    int getId() const { return id; }
    double getLongitude() const { return longitude; }
    double getLatitude() const { return latitude; }
};

#endif