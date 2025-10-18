#ifndef EDGE_H
#define EDGE_H

#include <cstdint>
class Edge
{
private:
    uint32_t idStart; // uint32_t since we expect a unique 32-bit unsigned number
    uint32_t idEnd;
    float weight;

public:
    /* Constructor */
    Edge(uint32_t idStart, uint32_t idEnd, float weight)
        : idStart(idStart), idEnd(idEnd), weight(weight) {}
    ~Edge() = default;

    /* Getters */
    float getWeight() const { return weight; }
    uint32_t getStartId() const { return idStart; }
    uint32_t getEndId() const { return idEnd; }
};

#endif