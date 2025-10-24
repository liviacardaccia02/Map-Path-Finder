#include "GraphicGraph.h"
#include "utils.h"
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <limits>
#include <QPen>
#include <QBrush>
#include <QPointF>
#include <QRectF>

GraphicGraph::GraphicGraph(const std::string &filename, QGraphicsScene *scene) : Graph(filename), graphicsScene(scene),
                                                                                 minLon(std::numeric_limits<double>::max()), maxLon(std::numeric_limits<double>::lowest()),
                                                                                 minLat(std::numeric_limits<double>::max()), maxLat(std::numeric_limits<double>::lowest())
{
    if (graphicsScene)
    {
        graphicsScene->setBackgroundBrush(Qt::black);
        if (graphicsScene->sceneRect().isNull())
        {
            graphicsScene->setSceneRect(-400, -300, 800, 600); // default visible area
        }
    }

    this->initializeFromFile(filename);
}

GraphicGraph::~GraphicGraph()
{
    Graph::~Graph();
    for (auto &item : vertexItems)
    {
        if (item.second)
            delete item.second;
    }
    for (auto &line : edgeItems)
    {
        if (line)
            delete line;
    }
}

void GraphicGraph::initializeFromFile(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        throw std::runtime_error("Error: could not open file " + filename);
    }

    int vertexCount = 0;
    int edgeCount = 0;
    std::string line;
    int lineNumber = 0;

    while (std::getline(file, line))
    {
        lineNumber++;

        if (line.empty() || line[0] == '#')
            continue; // skip empty lines or comments

        std::string_view sv(line);
        char type = sv.front();
        sv.remove_prefix(2); // remove "V," or "E,"

        if (type == 'V')
        {
            uint32_t id;
            double longitude, latitude;

            try
            {
                id = std::stoi(std::string(utils::nextField(sv)));
                longitude = std::stod(std::string(utils::nextField(sv)));
                latitude = std::stod(std::string(utils::nextField(sv)));
            }
            catch (const std::invalid_argument &e)
            {
                throw std::runtime_error("Parsing error at line " + std::to_string(lineNumber) +
                                         ": Invalid number format in vertex definition (possibly due to comments or malformed data).\n" +
                                         "Line content: " + line + "\n" +
                                         "Hint: Check for inline comments (//) or non-numeric characters.");
            }
            catch (const std::out_of_range &e)
            {
                throw std::runtime_error("Parsing error at line " + std::to_string(lineNumber) +
                                         ": Number out of range in vertex definition.\n" +
                                         "Line content: " + line);
            }

            Vertex v(id, longitude, latitude);
            this->addVertex(v);
            vertexCount++;
        }
        else if (type == 'E')
        {
            std::string parsedWeight;
            uint32_t idStart, idEnd;
            double weight;

            try
            {
                idStart = std::stoi(std::string(utils::nextField(sv)));
                idEnd = std::stoi(std::string(utils::nextField(sv)));
                parsedWeight = std::string(utils::nextField(sv));
                weight = parsedWeight.empty() || parsedWeight == "0" ? utils::computeEuclideanDistance(*this, getVertex(idStart), getVertex(idEnd))
                                                                     : std::stod(parsedWeight);
            }
            catch (const std::invalid_argument &e)
            {
                throw std::runtime_error("Parsing error at line " + std::to_string(lineNumber) +
                                         ": Invalid number format in edge definition (possibly due to comments or malformed data).\n" +
                                         "Line content: " + line + "\n" +
                                         "Hint: Check for inline comments (//) or non-numeric characters.");
            }
            catch (const std::out_of_range &e)
            {
                throw std::runtime_error("Parsing error at line " + std::to_string(lineNumber) +
                                         ": Number out of range in edge definition.\n" +
                                         "Line content: " + line);
            }

            Edge e(idStart, idEnd, weight);
            this->addEdge(e);
            edgeCount++;
        }
    }

    file.close();
}

void GraphicGraph::addVertex(const Vertex &vertex)
{
    Graph::addVertex(vertex);

    minLon = std::min(minLon, vertex.getLongitude());
    maxLon = std::max(maxLon, vertex.getLongitude());
    minLat = std::min(minLat, vertex.getLatitude());
    maxLat = std::max(maxLat, vertex.getLatitude());

    if (graphicsScene)
    {
        auto [x, y] = geoToSceneCoords(vertex.getLongitude(), vertex.getLatitude());
        // Draw vertex as white circle on black background
        QPen pen(Qt::white);
        pen.setWidth(0);
        QBrush brush(Qt::white);
        QGraphicsEllipseItem *item = graphicsScene->addEllipse(x - 4, y - 4, 8, 8, pen, brush);
        if (item)
        {
            item->setZValue(2); // above edges
            item->setScale(itemScale);
            vertexItems[vertex.getId()] = item;
        }
    }
}

void GraphicGraph::addEdge(const Edge &edge)
{
    Graph::addEdge(edge);

    if (!graphicsScene)
        return;

    auto itStart = vertexItems.find(edge.getStartId());
    auto itEnd = vertexItems.find(edge.getEndId());
    if (itStart == vertexItems.end() || itEnd == vertexItems.end())
        return;

    QGraphicsEllipseItem *startItem = itStart->second;
    QGraphicsEllipseItem *endItem = itEnd->second;
    if (!startItem || !endItem)
        return;

    // Use scene bounding rect centers to get reliable scene coordinates
    QPointF p1 = startItem->sceneBoundingRect().center();
    QPointF p2 = endItem->sceneBoundingRect().center();

    // Draw edge as thin white line
    QPen whitePen(Qt::white);
    whitePen.setWidth(1);
    QGraphicsLineItem *line = graphicsScene->addLine(p1.x(), p1.y(), p2.x(), p2.y(), whitePen);
    edgeItems.push_back(line);
}

void GraphicGraph::drawPath(const std::vector<uint32_t> &path) const
{
    if (!graphicsScene || path.size() < 2)
        return;

    // Clear previous path items
    for (auto it : pathItems)
    {
        if (it)
            graphicsScene->removeItem(it);
        delete it;
    }
    const_cast<GraphicGraph *>(this)->pathItems.clear();

    QPen pathPen(Qt::red);
    pathPen.setWidth(3);

    for (size_t i = 0; i < path.size() - 1; ++i)
    {
        uint32_t id1 = path[i];
        uint32_t id2 = path[i + 1];

        auto it1 = vertexItems.find(id1);
        auto it2 = vertexItems.find(id2);
        if (it1 == vertexItems.end() || it2 == vertexItems.end())
            continue;

        QGraphicsEllipseItem *item1 = it1->second;
        QGraphicsEllipseItem *item2 = it2->second;
        if (!item1 || !item2)
            continue;

        QPointF p1 = item1->sceneBoundingRect().center();
        QPointF p2 = item2->sceneBoundingRect().center();

        QGraphicsLineItem *line = graphicsScene->addLine(p1.x(), p1.y(), p2.x(), p2.y(), pathPen);
        if (line)
        {
            line->setZValue(10); // on top
            const_cast<GraphicGraph *>(this)->pathItems.push_back(line);
        }
    }
}

void GraphicGraph::zoomIn(double factor)
{
    setScale(itemScale * factor);
}

void GraphicGraph::zoomOut(double factor)
{
    setScale(itemScale / factor);
}

void GraphicGraph::setScale(double scale)
{
    if (scale <= 0)
        return;
    itemScale = scale;
    // Apply to vertex items; edges are lines so keep thickness constant by scaling pen width instead
    for (auto &kv : vertexItems)
    {
        if (kv.second)
            kv.second->setScale(itemScale);
    }
    // Optionally scale edges by changing their pen width
    for (auto &line : edgeItems)
    {
        if (line)
        {
            QPen p = line->pen();
            p.setWidthF(std::max(1.0, 1.0 * itemScale));
            line->setPen(p);
        }
    }
    // Also adjust existing path items
    for (auto &line : pathItems)
    {
        if (line)
        {
            QPen p = line->pen();
            p.setWidthF(std::max(2.0, 3.0 * itemScale));
            line->setPen(p);
        }
    }
}

std::pair<double, double> GraphicGraph::geoToSceneCoords(double longitude, double latitude)
{
    if (minLon == std::numeric_limits<double>::max())
        return {0, 0};

    double lonRange = maxLon - minLon;
    double latRange = maxLat - minLat;
    if (lonRange <= 0.0)
        lonRange = 1.0;
    if (latRange <= 0.0)
        latRange = 1.0;

    // Map geographic bounding box into the scene rect with margins
    QRectF rect = graphicsScene ? graphicsScene->sceneRect() : QRectF(-400, -300, 800, 600);
    double margin = 20.0;
    double width = rect.width() - 2 * margin;
    double height = rect.height() - 2 * margin;
    if (width <= 0)
        width = 800;
    if (height <= 0)
        height = 600;

    double x = rect.left() + margin + ((longitude - minLon) / lonRange) * width;
    double y = rect.top() + margin + ((maxLat - latitude) / latRange) * height;
    return {x, y};
}
