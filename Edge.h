#ifndef EDGE_H
#define EDGE_H

class Edge
{
private:
    int idStart;
    int idEnd;
    float weight;

public:
    /* Constructor */
    Edge(int idStart, int idEnd, float weight)
        : idStart(idStart), idEnd(idEnd), weight(weight) {}
    ~Edge() = default;

    /* Getters */
    float getWeight() const { return weight; }
    int getStartId() const { return idStart; }
    int getEndId() const { return idEnd; }
};

#endif