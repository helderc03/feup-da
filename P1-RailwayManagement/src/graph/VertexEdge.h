#ifndef DA_RAILWAYMANAGEMENT_VERTEXEDGE_H
#define DA_RAILWAYMANAGEMENT_VERTEXEDGE_H

#include <vector>
#include "Station.h"

class Edge;

class Vertex {
private:
    Station station;
    std::vector<Edge *> edges;
    bool visited = false;
    bool processing = false;
    unsigned int inDegree;
    int distance;
    Edge *path = nullptr;
    std::vector<Edge *> incoming;

public:
    explicit Vertex(const Station &station);

    const Station &getStation() const;

    std::vector<Edge *> getEdges() const;

    bool isVisited() const;

    int getDistance() const;

    Edge *getPath() const;

    std::vector<Edge *> getIncoming() const;

    void setVisited(bool _visited);

    void setPath(Edge *_path);

    void setDistance(int _distance);

    /**
     * Adds a new outgoing edge to the vertex/station
     * Time Complexity: O(1)
     * @param dest Destination of the edge
     * @param weight Weight of the edge
     * @param service Service of the edge
     * @return Pointer to the new edge
     */
    Edge *addEdge(Vertex *dest, int weight, const std::string &service);

    /**
     * Removes an edge with a given destination from the vertex/station
     * Time Complexity: O(i*o) being i the number of incoming edges and o the number of outgoing edges
     * @param des Station to remove the edge to
     * @return True if the edge was removed, false otherwise
     */
    bool removeEdge(const Station &dest);

    bool operator==(const Vertex &vertex) const;

};

class Edge {
private:
    Vertex *origin;
    Vertex *dest;
    int weight;
    Edge *reverse = nullptr;
    int flow = 0;
    int cost = 0;
    std::string service;

public:
    Edge(Vertex *origin, Vertex *dest, int weight, const std::string &service);

    Vertex *getOrigin() const;

    Vertex *getDest() const;

    int getWeight() const;

    int getFlow() const;

    int getCost() const;

    std::string getService() const;

    void setReverse(Edge *_reverse);

    void setFlow(int _flow);

    void setCost(int _cost);

    bool operator==(const Edge &edge) const;
};

#endif //DA_RAILWAYMANAGEMENT_VERTEXEDGE_H
