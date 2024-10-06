#include "DatasetLoader.h"
#include <iostream>
#include <fstream>
#include <sstream>

Graph* loadSampleGraph(const std::string& filePath, bool skipFirstLine) {
    std::ifstream file(filePath);

    if (!file.good()) {
        std::cout << "Error: File not found." << std::endl;
        return nullptr;
    }

    auto *graph = new Graph();

    std::string line;

    if (skipFirstLine) {
        std::getline(file, line);
    }

    while (std::getline(file, line).good()) {
        if (line.back() == '\r') line.pop_back();
        std::stringstream iss(line);

        std::string origin;
        std::string destination;
        std::string distance;
        std::string labelOrigin;
        std::string labelDest;
        bool hasLabels = false;

        std::getline(iss, origin, ',');
        std::getline(iss, destination, ',');
        std::getline(iss, distance, ',');
        if (std::getline(iss, labelOrigin, ',')) {
            std::getline(iss, labelDest, ',');
            hasLabels = true;
        }

        if (graph->findVertex(stoi(origin)) == nullptr) {
            graph->addVertex(stoi(origin));
            if (hasLabels) {
                graph->findVertex(stoi(origin))->setLabel(labelOrigin);
            }
        }

        if (graph->findVertex(stoi(destination)) == nullptr) {
            graph->addVertex(stoi(destination));
            if (hasLabels) {
                graph->findVertex(stoi(destination))->setLabel(labelDest);
            }
        }

        int originId = stoi(origin);
        int destId = stoi(destination);
        double dist = stod(distance);
        graph->addBidirectionalEdge(originId, destId, dist);
    }
    return graph;
}


Graph *loadRealWorldGraph(const std::string& nodesFilePath, const std::string& edgesFilePath) {
    std::ifstream nodesFile(nodesFilePath);
    std::ifstream edgesFile(edgesFilePath);

    if (!nodesFile.good() || !edgesFile.good()) {
        std::cout << "Error: File not found." << std::endl;
        return nullptr;
    }

    auto *graph = new Graph();

    std::string line;
    std::getline(nodesFile, line);

    while (std::getline(nodesFile, line).good()) {
        if (line.back() == '\r') line.pop_back();
        std::stringstream iss(line);

        std::string id;
        std::string lon;
        std::string lat;

        std::getline(iss, id, ',');
        std::getline(iss, lon, ',');
        std::getline(iss, lat, ',');

        graph->addVertex(stoi(id));
        graph->findVertex(stoi(id))->setLon(stod(lon));
        graph->findVertex(stoi(id))->setLat(stod(lat));
    }

    std::getline(edgesFile, line);

    while (std::getline(edgesFile, line).good()) {
        if (line.back() == '\r') line.pop_back();
        std::stringstream iss(line);

        std::string origin;
        std::string destination;
        std::string distance;

        std::getline(iss, origin, ',');
        std::getline(iss, destination, ',');
        std::getline(iss, distance, ',');

        int originId = stoi(origin);
        int destId = stoi(destination);
        double dist = stod(distance);

        graph->addBidirectionalEdge(originId, destId, dist);
    }

    return graph;
}
