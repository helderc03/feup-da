#include "VertexEdge.h"

Vertex::Vertex(const Station &station) {
    this->station = station;
}

const Station &Vertex::getStation() const {
    return station;
}

std::vector<Edge *> Vertex::getEdges() const {
    return edges;
}

bool Vertex::isVisited() const {
    return visited;
}

int Vertex::getDistance() const { return distance; }

Edge *Vertex::getPath() const {
    return path;
}

std::vector<Edge *> Vertex::getIncoming() const {
    return incoming;
}

void Vertex::setPath(Edge *_path) {
    this->path = _path;
}

void Vertex::setDistance(int _distance) {
    this->distance = _distance;
}

Edge *Vertex::addEdge(Vertex *dest, int weight, const std::string &service) {
    Edge *edge = new Edge(this, dest, weight, service);
    edges.push_back(edge);
    dest->incoming.push_back(edge);
    return edge;
}

bool Vertex::removeEdge(const Station &dest) {
    bool removed = false;
    for (auto it = edges.begin(); it != edges.end();) {
        Edge *edge = *it;
        Vertex *v = edge->getDest();
        if (v->getStation() == dest) {
            it = edges.erase(it);
            for (auto it2 = v->incoming.begin(); it2 != v->incoming.end();) {
                if ((*it2)->getOrigin()->getStation() == station) {
                    it2 = v->incoming.erase(it2);
                } else it2++;
            }
            delete edge;
            removed = true;
        } else it++;
    }
    return removed;
}

bool Vertex::operator==(const Vertex &vertex) const {
    return station == vertex.station;
}

Edge::Edge(Vertex *origin, Vertex *dest, int weight, const std::string &service) {
    this->origin = origin;
    this->dest = dest;
    this->weight = weight;
    this->service = service;
}

Vertex *Edge::getOrigin() const {
    return origin;
}

Vertex *Edge::getDest() const {
    return dest;
}

int Edge::getWeight() const {
    return weight;
}

int Edge::getFlow() const {
    return flow;
}

int Edge::getCost() const {
    return cost;
}

std::string Edge::getService() const {
    return service;
}

void Edge::setReverse(Edge *_reverse) {
    this->reverse = _reverse;
}

void Edge::setFlow(int _flow) {
    this->flow = _flow;
}

void Edge::setCost(int _cost) {
    this->cost = _cost;
}

void Vertex::setVisited(bool _visited) {
    this->visited = _visited;
}

bool Edge::operator==(const Edge &edge) const {
    return origin == edge.origin && dest == edge.dest && weight == edge.weight && service == edge.service;
}



