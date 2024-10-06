#include "DatasetLoader.h"
#include <iostream>
#include <fstream>
#include <sstream>

Graph *loadDataset(std::string networkPath, std::string stationsPath) {
    std::ifstream networkFile(networkPath);
    std::ifstream stationFile(stationsPath);

    if (!networkFile.good()) {
        std::cout << "Couldn't load network file " << networkPath << "\n";
        return nullptr;
    }

    if (!stationFile.good()) {
        std::cout << "Couldn't load stations file " << stationsPath << "\n";
        return nullptr;
    }

    Graph *graph = new Graph();

    std::string stationLine;
    std::getline(stationFile, stationLine);

    while (std::getline(stationFile, stationLine).good()) {
        std::istringstream stationLineStream(stationLine);
        std::string name;
        std::getline(stationLineStream, name, ',');
        std::string district;
        std::getline(stationLineStream, district, ',');
        std::string municipality;
        std::getline(stationLineStream, municipality, ',');
        std::string township;
        std::getline(stationLineStream, township, ',');
        std::string line;
        std::getline(stationLineStream, line, '\r');
        Station station = Station(name, district, municipality, township, line);

        if (graph->findVertex(name) != nullptr) {
            std::cout << name << " already exists in graph... skipping it.\n";
            continue;
        }
        graph->addVertex(station);
    }

    std::string networkLine;
    std::getline(networkFile, networkLine);

    while (std::getline(networkFile, networkLine).good()) {
        std::istringstream networkLineStream(networkLine);
        std::string stationA;
        std::getline(networkLineStream, stationA, ',');
        std::string stationB;
        std::getline(networkLineStream, stationB, ',');
        std::string capacityStr;
        std::getline(networkLineStream, capacityStr, ',');
        int capacity = std::atoi(capacityStr.c_str());
        std::string service;
        std::getline(networkLineStream, service, '\r');
        Vertex *src = graph->findVertex(stationA);
        Vertex *dst = graph->findVertex(stationB);

        if (src == nullptr) {
            std::cout << stationA << " doesnt exist in graph... skipping it.\n";
            continue;
        }

        if (dst == nullptr) {
            std::cout << stationA << " doesnt exist in graph... skipping it.\n";
            continue;
        }

        bool edgeExists = false;
        for (Edge *e: src->getEdges()) {
            if (e->getDest() == dst) {
                std::cout << "Edge " << stationA << " -> " << stationB << " already exists in graph... skipping it.\n";
                edgeExists = true;
            }
        }

        if (!edgeExists) graph->addBidirectionalEdge(stationA, stationB, capacity, service);
    }
    return graph;
}
