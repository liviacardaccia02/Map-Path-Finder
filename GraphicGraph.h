#ifndef GRAPHICGRAPH_H
#define GRAPHICGRAPH_H

#include "Graph.h"
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>

class GraphicGraph : public Graph
{
public:
    /* Constructor & destructur */
    GraphicGraph(const std::string &filename, QGraphicsScene *scene);
    ~GraphicGraph();

    /**
     * Helper method to initialize the graph from a file.
     * Extends the base class method for graphical setup.
     * @param filename The name of the file containing the graph data.
     */
    void initializeFromFile(const std::string &filename) override;

    /**
     * Adds a vertex to the graph and creates its graphical representation.
     * @param id The ID of the vertex.
     * @param longitude The longitude of the vertex.
     * @param latitude The latitude of the vertex.
     */
    void addVertex(const Vertex &vertex) override;

    /**
     * Adds an edge to the graph and creates its graphical representation.
     * @param idStart The starting vertex ID of the edge.
     * @param idEnd The ending vertex ID of the edge.
     * @param weight The weight of the edge.
     */
    void addEdge(const Edge &edge) override;

    /**
     * Draws the path on the graphics scene by highlighting the edges along the given path.
     * @param path A vector of vertex IDs representing the path to be drawn.
     */
    void drawPath(const std::vector<uint32_t> &path) const override;

    // Simple programmatic zoom controls (scale items) — call from your view/controller
    void zoomIn(double factor = 1.25);
    void zoomOut(double factor = 1.25);
    void setScale(double scale);

private:
    mutable QGraphicsScene *graphicsScene;                            // Graphics scene for rendering it's mutable to allow drawing in const method
    double minLon, maxLon, minLat, maxLat;                            // Convertion limits
    std::unordered_map<uint32_t, QGraphicsEllipseItem *> vertexItems; // Ellipse items for vertices
    std::vector<QGraphicsLineItem *> edgeItems;                       // Line items for edges

    std::pair<double, double> geoToSceneCoords(double longitude, double latitude);

    // items used to draw the current path (so they can be cleared/redrawn)
    mutable std::vector<QGraphicsLineItem *> pathItems;
    // current scale applied to items
    double itemScale = 1.0;
};

#endif