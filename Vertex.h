#ifndef VERTEX_H
#define VERTEX_H

class Vertex
{
private:
    int id;
    float latitude, longitude;

public:
    /* Constructor */
    Vertex(int id, float latitude, float longitude) : id(id), latitude(latitude), longitude(longitude) {}
    ~Vertex() = default;

    /* Getters */
    int getId() const { return id; }
    float getLatitude() const { return latitude; }
    float getLongitude() const { return longitude; }
};

#endif