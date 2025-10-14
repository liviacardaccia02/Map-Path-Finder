#ifndef EDGE_H
#define EDGE_H

#include "Vertex.h"

class Edge
{
private:
    int id;
    Vertex start;
    Vertex end;
    float weight;

public:
    /* Constructor */
    Edge(int id, const Vertex &start, const Vertex &end, float weight)
        : id(id), start(start), end(end), weight(weight) {}
    ~Edge() = default;

    /* Getters */
    int getId() const { return id; }
    float getWeight() const { return weight; }
    Vertex getStart() const { return start; }
    Vertex getEnd() const { return end; }
};

#endif