#include <unordered_map>
#include <algorithm>
#include <cmath>
#include "Graph.h"
#include "utils.h"

Vertex *Graph::findVertex(const std::string &station) const {
    for (Vertex *v: vertexSet) {
        if (v->getStation().getName() == station) {
            return v;
        }
    }
    return nullptr;
}

bool Graph::addVertex(const Station &station) {
    if (findVertex(station.getName()) != nullptr)
        return false;
    vertexSet.push_back(new Vertex(station));
    return true;
}

bool Graph::addEdge(const std::string &origin, const std::string &dest, int weight, const std::string &service) {
    Vertex *v1 = findVertex(origin);
    Vertex *v2 = findVertex(dest);
    if (v1 == nullptr || v2 == nullptr)
        return false;
    v1->addEdge(v2, weight, service);
    return true;
}

bool Graph::addBidirectionalEdge(const std::string &origin, const std::string &dest, int weight,
                                 const std::string &service) {
    Vertex *v1 = findVertex(origin);
    Vertex *v2 = findVertex(dest);
    if (v1 == nullptr || v2 == nullptr)
        return false;

    Edge *e1 = v1->addEdge(v2, weight, service);
    Edge *e2 = v2->addEdge(v1, weight, service);
    e1->setReverse(e2);
    e2->setReverse(e1);
    return true;
}

int Graph::getNumVertex() const {
    return vertexSet.size();
}

std::vector<Vertex *> Graph::getVertexSet() const {
    return vertexSet;
}

void Graph::DFS(Vertex *source) {
    source->setVisited(true);
    for (auto e: source->getEdges()) {
        if (!e->getDest()->isVisited()) {
            DFS(e->getDest());
        }
    }
}

void Graph::resetVisited() {
    for (auto v: vertexSet) {
        v->setVisited(false);
    }
}

void Graph::resetFlow() {
    for (auto v: vertexSet) {
        for (auto e: v->getEdges()) {
            e->setFlow(0);
        }
    }
}

void Graph::testAndVisit(std::queue<Vertex *> &q, Edge *e, Vertex *w, float residual) {
    if (!w->isVisited() && residual > 0) {
        w->setVisited(true);
        w->setPath(e);
        q.push(w);
    }
}

int Graph::findMinResidualAlongPath(Vertex *source, Vertex *dest) {
    int minResidual = INT_MAX;
    for (Vertex *v = dest; v != source;) {
        Edge *e = v->getPath();
        if (e->getDest() == v) {
            minResidual = std::min(minResidual, e->getWeight() - e->getFlow());
            v = e->getOrigin();
        } else {
            minResidual = std::min(minResidual, e->getFlow());
            v = e->getDest();
        }
    }
    return minResidual;
}

bool Graph::findAugmentingPath(Vertex *source, Vertex *dest) {
    for (Vertex *v: vertexSet) {
        v->setVisited(false);
    }
    source->setVisited(true);
    std::queue<Vertex *> q;
    q.push(source);
    while (!q.empty() && !dest->isVisited()) {
        Vertex *v = q.front();
        q.pop();
        for (Edge *e: v->getEdges()) {
            testAndVisit(q, e, e->getDest(), e->getWeight() - e->getFlow());
        }
        for (Edge *e: v->getIncoming()) {
            testAndVisit(q, e, e->getOrigin(), e->getFlow());
        }
    }
    return dest->isVisited();
}

void Graph::augmentFlowAlongPath(Vertex *source, Vertex *dest, int minResidual) {
    for (Vertex *v = dest; v != source;) {
        Edge *e = v->getPath();
        if (e->getDest() == v) {
            e->setFlow(e->getFlow() + minResidual);
            v = e->getOrigin();
        } else {
            e->setFlow(e->getFlow() - minResidual);
            v = e->getDest();
        }
    }
}

int Graph::edmondsKarp(Vertex *source, Vertex *dest) {
    if (source == nullptr || dest == nullptr || source == dest) {
        return -1;
    }

    resetFlow();

    int max_flow = 0;
    while (findAugmentingPath(source, dest)) {
        int pathFlow = INT_MAX;
        pathFlow = findMinResidualAlongPath(source, dest);
        augmentFlowAlongPath(source, dest, pathFlow);
        max_flow += pathFlow;
    }

    resetFlow();

    return max_flow;
}

int Graph::edmondsKarpSinkOnly(Vertex *dest) {
    Station superSourceStation("superSource", "", "", "", "");
    addVertex(superSourceStation);

    resetFlow();

    for (auto v: vertexSet) {
        if (v->getEdges().size() == 1 && v != dest)
            addEdge(superSourceStation.getName(), v->getStation().getName(), INT_MAX, "");
    }
    Vertex *superSource = findVertex(superSourceStation.getName());
    int maxFlow = edmondsKarp(superSource, dest);

    for (Edge *e: superSource->getEdges()) {
        Vertex *v = e->getDest();
        superSource->removeEdge(v->getStation());
    }
    vertexSet.pop_back();

    resetFlow();

    return maxFlow;
}

std::pair<std::vector<std::pair<Vertex *, Vertex *>>, int> Graph::moreDemandingPairOfStations() {
    int max = INT_MIN;
    std::vector<std::pair<Vertex *, Vertex *>> maxStations;

    for (auto it1 = vertexSet.begin(); it1 != vertexSet.end(); it1++) {
        for (auto it2 = it1 + 1; it2 != vertexSet.end(); it2++) {
            Vertex *v1 = *it1;
            Vertex *v2 = *it2;
            resetVisited();
            DFS(v1);
            if (v2->isVisited()) {
                int thisFlow = edmondsKarp(v1, v2);
                if (thisFlow > max) {
                    max = thisFlow;
                    maxStations.clear();
                    maxStations.push_back(std::make_pair(v1, v2));
                } else if (thisFlow == max) {
                    maxStations.push_back(std::make_pair(v1, v2));
                }
            }
        }
    }
    return std::make_pair(maxStations, max);
}

void Graph::findTopK(std::vector<std::pair<std::string, float>> &maxFlowWeightedAverage,
                     std::vector<std::pair<std::string, int>> &highestBottleneck,
                     std::vector<std::pair<std::string, int>> &moreStations,
                     int k, bool useDistricts) {

    std::unordered_map<std::string, int> stationsPerArea;
    std::unordered_map<std::string, int> bottlenecks;
    std::unordered_map<std::string, std::vector<int>> flowsPerStation;

    for (auto v: vertexSet) {
        std::string area = useDistricts ? v->getStation().getDistrict() : v->getStation().getMunicipality();
        stationsPerArea[area] = 0;
        bottlenecks[area] = 0;
        flowsPerStation[area] = std::vector<int>();
    }

    for (auto v: vertexSet) {
        int maxFlow = edmondsKarpSinkOnly(v);
        std::string area = useDistricts ? v->getStation().getDistrict() : v->getStation().getMunicipality();
        if (maxFlow > bottlenecks[area]) bottlenecks[area] = maxFlow;
        stationsPerArea[area] += 1;
        flowsPerStation[area].push_back(maxFlow);
    }

    auto compare = [](std::pair<std::string, int> &a, std::pair<std::string, int> &b) {
        return a.second > b.second;
    };

    auto compared = [](std::pair<std::string, float> &a, std::pair<std::string, float> &b) {
        return a.second > b.second;
    };

    std::priority_queue<std::pair<std::string, float>, std::vector<std::pair<std::string, float>>, decltype(compared)> pqd(
            compared);
    std::priority_queue<std::pair<std::string, int>, std::vector<std::pair<std::string, int>>, decltype(compare)> pq(
            compare);

    for (const std::pair<std::string, std::vector<int>> area: flowsPerStation) {
        std::vector<float> weights(area.second.size());
        float weightSum = computeWeights(area.second, weights);
        float weightedAverage = 0;
        for (int i = 0; i < area.second.size(); i++) {
            weightedAverage += weights[i] * area.second[i];
        }
        weightedAverage /= weightSum;
        weightedAverage = std::round(weightedAverage * 100) / 100;
        pqd.push(std::make_pair(area.first, weightedAverage));
        if (pqd.size() > k) pqd.pop();
    }

    while (!pqd.empty()) {
        maxFlowWeightedAverage.emplace_back(pqd.top());
        pqd.pop();
    }

    for (const std::pair<std::string, int> area: bottlenecks) {
        pq.push(area);
        if (pq.size() > k) pq.pop();
    }

    while (!pq.empty()) {
        highestBottleneck.push_back(pq.top());
        pq.pop();
    }

    for (const std::pair<std::string, int> area: stationsPerArea) {
        pq.push(area);
        if (pq.size() > k) pq.pop();
    }

    while (!pq.empty()) {
        moreStations.push_back(pq.top());
        pq.pop();
    }

    std::sort(maxFlowWeightedAverage.begin(), maxFlowWeightedAverage.end(),
              [](const std::pair<std::string, float> &a, const std::pair<std::string, float> &b) {
                  return a.second > b.second;
              });
    std::sort(highestBottleneck.begin(), highestBottleneck.end(),
              [](const std::pair<std::string, int> &a, const std::pair<std::string, int> &b) {
                  return a.second > b.second;
              });
    std::sort(moreStations.begin(), moreStations.end(),
              [](const std::pair<std::string, int> &a, const std::pair<std::string, int> &b) {
                  return a.second > b.second;
              });
}
