#include <unordered_map>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <set>
#include "Graph.h"
#include "Utils.h"

Vertex* Graph::findVertex(int id) const {
    auto it = vertexSet.find(id);
    if (it == vertexSet.end())
        return nullptr;
    return it->second;
}

bool Graph::addVertex(int id) {
    auto it = vertexSet.find(id);
    if (it != vertexSet.end())
        return false;
    auto *v1 = new Vertex(id);
    vertexSet.insert({id, v1});
    return true;
}

bool Graph::addEdge(int origin, int dest, double weight) {
    Vertex *v1 = findVertex(origin);
    Vertex *v2 = findVertex(dest);
    if (v1 == nullptr || v2 == nullptr)
        return false;
    v1->addEdge(v2, weight);
    return true;
}

bool Graph::addBidirectionalEdge(int origin, int dest, double weight) {
    Vertex *v1 = findVertex(origin);
    Vertex *v2 = findVertex(dest);
    if (v1 == nullptr || v2 == nullptr)
        return false;

    Edge *e1 = v1->addEdge(v2, weight);
    Edge *e2 = v2->addEdge(v1, weight);
    e1->setReverse(e2);
    e2->setReverse(e1);
    return true;
}

int Graph::getNumVertex() const {
    return (int) vertexSet.size();
}

std::unordered_map<int, Vertex *> Graph::getVertexSet() const {
    return vertexSet;
}

void Graph::resetVisited() {
    for (auto v: vertexSet) {
        v.second->setVisited(false);
    }
}

void Graph::tsp_backtracking(std::vector<int> &path, std::vector<int> &bestPath, double &minCost, double cumulatedCost) {
    int first = path.front();
    int last = path.back();

    if (path.size() == vertexSet.size()) {
        for (Edge* e : findVertex(last)->getEdges()) {
            Vertex* v = e->getDest();
            if (v->getId() == first) {
                double cycleCost = cumulatedCost + e->getWeight();
                if (cycleCost < minCost) {
                    minCost = cycleCost;
                    bestPath = path;
                }
                break;
            }
        }
        return;
    }

    for (Edge* e : findVertex(last)->getEdges()) {
        Vertex* v = e->getDest();
        if (!v->isVisited()) {
            double new_cost = cumulatedCost + e->getWeight();
            path.push_back(v->getId());
            v->setVisited(true);
            tsp_backtracking(path, bestPath, minCost, new_cost);
            path.pop_back();
            v->setVisited(false);
        }
    }
}

void Graph::MSTPrim(std::vector<Edge*> &edges) {

    for (auto v : vertexSet) {
        v.second->setDistance(std::numeric_limits<double>::max());
        v.second->setVisited(false);
        v.second->setPath(nullptr);
    }

    Vertex* first = vertexSet.find(0)->second;
    first->setDistance(0);

    MutablePriorityQueue<Vertex> q;
    q.insert(first);

    while (!q.empty()) {
        Vertex *v = q.extractMin();
        v->setVisited(true);
        for (Edge *e: v->getEdges()) {
            Vertex *w = e->getDest();
            if (!w->isVisited()) {
                auto oldDist = w->getDistance();
                if (e->getWeight() < oldDist) {
                    w->setDistance(e->getWeight());
                    w->setPath(e);
                    if (oldDist == std::numeric_limits<double>::max()) {
                        q.insert(w);
                    } else {
                        q.decreaseKey(w);
                    }
                }
            }
        }
    }

    for (auto v : vertexSet) {
        if (v.second->getPath() != nullptr) {
            edges.push_back(v.second->getPath());
        }
    }

    // Sort edges by weight
    std::sort(edges.begin(), edges.end(), [](Edge* e1, Edge* e2) {
        return e1->getWeight() < e2->getWeight();
    });
}


double Graph::triangularInequalityHeuristic() {
    std::vector<Vertex*> path;
    std::vector<Edge*> edges;
    resetVisited();
    MSTPrim(edges);

    resetVisited();
    Vertex* first = vertexSet.find(0)->second;
    preOrderTraversal(edges, first, path);

    double distance = computePathDistance(path);
    return distance;
}

bool Graph::connectedNodes(int id1, int id2) const {
    Vertex* v1 = findVertex(id1);
    Vertex* v2 = findVertex(id2);
    if (v1 == nullptr || v2 == nullptr) {
        return false;
    }

    for (const Edge* e : v1->getEdges()) {
        if (e->getDest()->getId() == v2->getId()) {
            return true;
        }
    }

    return false;
}

double Graph::computePathDistance(std::vector<Vertex *> &path) {
    double distance = 0.0;

    for (int i = 0; i < path.size() - 1; ++i) {
        Vertex* v1 = path[i];
        Vertex* v2 = path[i + 1];

        if (!connectedNodes(v1->getId(), v2->getId())) {
            distance += haversineDistance(v1, v2);
            continue;
        }

        for (Edge* e : v1->getEdges()) {
            if (e->getDest()->getId() == v2->getId()) {
                distance += e->getWeight();
                break;
            }
        }
    }

    int last = path.back()->getId();
    int first = path.front()->getId();

    if (!connectedNodes(last, first)) {
        distance += haversineDistance(findVertex(last), findVertex(first));
    }
    else {
        for (Edge* e : path.back()->getEdges()) {
            if (e->getDest()->getId() == first) {
                distance += e->getWeight();
                break;
            }
        }
    }

    return distance;
}

void Graph::preOrderTraversal(const std::vector<Edge *>& mst, Vertex *v, std::vector<Vertex *> &path) {
    v->setVisited(true);

    //std::cout << v->getId() << " ";
    path.push_back(v);

    for (Edge* e : mst) {
        if (e->getOrigin()->getId() == v->getId() && !e->getDest()->isVisited()) {
            preOrderTraversal(mst, e->getDest(), path);
        }
    }
}

double Graph::triangularInequalityHeuristic2Opt()
{
    std::vector<Vertex*> path;
    std::vector<Edge*> edges;
    resetVisited();
    MSTPrim(edges);

    resetVisited();
    Vertex* first = vertexSet.find(0)->second;
    preOrderTraversal(edges, first, path);

    double distance = computePathDistance(path);
    twoOpt(path, distance);

    return distance;
}

void Graph::twoOpt(std::vector<Vertex *> &path, double &distance)
{
    bool foundImprovement = true;
    while(foundImprovement){
        foundImprovement = false;
        for(int i = 0; i < vertexSet.size() - 2; i++){
            for(int j = i+1; j < vertexSet.size() - 1; j++){

                if(matrixGraph[path[i]->getId()][path[i+1]->getId()] == 0 
                    || matrixGraph[path[j]->getId()][path[j+1]->getId()]==0
                    || matrixGraph[path[i]->getId()][path[j]->getId()] == 0
                    || matrixGraph[path[i+1]->getId()][path[j+1]->getId()] == 0){
                        //this edge doesnt exist so we ignore it
                    continue;

                }
                int lengthDelta = -matrixGraph[path[i]->getId()][path[i+1]->getId()] 
                    -matrixGraph[path[j]->getId()][path[j+1]->getId()]
                    +matrixGraph[path[i]->getId()][path[j]->getId()] 
                    +matrixGraph[path[i+1]->getId()][path[j+1]->getId()];
                if(lengthDelta < 0){
                    foundImprovement = true;
                    std::reverse(path.begin() + i + 1, path.begin()+j+1);
                    distance += lengthDelta;
                }

            }
        }
    }
}

double Graph::localSearch2Opt()
{
    std::vector<Vertex*> path;
    for(int i = 0; i < vertexSet.size(); i++){
        path.push_back((*vertexSet.find(i)).second);
    }   

    double distance = computePathDistance(path);

    twoOpt(path, distance);

    return distance;

}


double Graph::simulatedAnnealing2Opt(double initialTemperature, int steps, double cooldownRate)
{
    //use 2-approximation as basis, since it's unnecessary to start from a random solution
    std::vector<Vertex*> path;
    std::vector<Edge*> edges;
    resetVisited();
    MSTPrim(edges);


    resetVisited();
    Vertex* first = vertexSet.find(0)->second;
    preOrderTraversal(edges, first, path);
    double distance = computePathDistance(path);
    twoOpt(path, distance);

    std::vector<Vertex*> globalBestPath = path;
    double globalBestDistance = distance;

    int totalSteps = 0;

    //a step is a change in distance either a better one or a worse one
    bool foundSolution = true;
    while (totalSteps < steps)
    {
        for(int i = 0; i < vertexSet.size() - 2; i++){
            for(int j = 0; j < vertexSet.size() - 1; j++){
                if(i == j) continue;

                if(matrixGraph[path[i]->getId()][path[i+1]->getId()] == 0 
                    || matrixGraph[path[j]->getId()][path[j+1]->getId()]==0
                    || matrixGraph[path[i]->getId()][path[j]->getId()] == 0
                    || matrixGraph[path[i+1]->getId()][path[j+1]->getId()] == 0){
                        //this edge doesnt exist so we ignore it
                    continue;

                }
                int lengthDelta = -matrixGraph[path[i]->getId()][path[i+1]->getId()] 
                    -matrixGraph[path[j]->getId()][path[j+1]->getId()]
                    +matrixGraph[path[i]->getId()][path[j]->getId()] 
                    +matrixGraph[path[i+1]->getId()][path[j+1]->getId()];
                if(lengthDelta < 0){
                    std::reverse(path.begin() + i + 1, path.begin()+j+1);
                    distance += lengthDelta;
                    //decrease current temperature
                    if(distance <= globalBestDistance){
                        globalBestDistance = distance;
                        globalBestPath = path;
                        totalSteps = 0;
                        foundSolution = true;
                    }
                    break;
                } else {
                    //make probability for worse path
                    //if accepted, use worse path increment totalSteps and decrease current temperature
                    totalSteps++;
                    if(lengthDelta == 0 || !foundSolution) continue;
                    double random = ((double) rand() / RAND_MAX);
                    double prob = exp(-(double)lengthDelta/initialTemperature);
                    if(random < prob){
                        //std::cout << "difference: " << lengthDelta << "\n";
                        std::reverse(path.begin() + i + 1, path.begin()+j+1);
                        distance += lengthDelta;
                        initialTemperature = initialTemperature*cooldownRate; 
                        totalSteps++;
                        foundSolution = false;
                        break;
                    }
                }


            }
        }
    }
    //maybe after all steps, run the 2opt again to ensure the best solution, not sure if needed
    twoOpt(path, distance);
    if(distance < globalBestDistance){
        globalBestDistance = distance;
        globalBestPath = path;
    }
    
    return globalBestDistance;

}

void Graph::initDistanceGraph() {
    matrixGraph = new double*[vertexSet.size()];
    for(int i = 0; i < vertexSet.size(); i++){
        matrixGraph[i] = new double[vertexSet.size()];
        for(int j = 0; j < vertexSet.size(); j++){
            if(i == j){
                matrixGraph[i][j] = std::numeric_limits<double>::max();
            }
            matrixGraph[i][j] = 0;
        }
    } 
    for(auto vertex : vertexSet){
        for(auto edge : vertex.second->getEdges()){
            matrixGraph[vertex.second->getId()][edge->getDest()->getId()] = edge->getWeight();
        }
    }
    matrixGraphInitialized = true;
}

void Graph::freeDistanceGraph() {
    for(int i = 0; i < vertexSet.size(); i++){
        delete[] matrixGraph[i];
    }
    delete[] matrixGraph;
    matrixGraphInitialized = false;
}
