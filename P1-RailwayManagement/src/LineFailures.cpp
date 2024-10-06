#include "LineFailures.h"
#include <iostream>
#include <tuple>
#include <algorithm>
#include <cmath>

int maximumTrainsReducedConnectivity(Graph *graph, std::vector<Edge *> segmentsToRemove, Vertex *src, Vertex *dst) {
    graph->resetVisited();
    graph->resetFlow();

    std::vector<std::tuple<std::string, std::string, int, std::string>> segments;
    segments.reserve(segmentsToRemove.size());
    for (auto segment: segmentsToRemove) {
        segments.emplace_back(
                segment->getOrigin()->getStation().getName(),
                segment->getDest()->getStation().getName(),
                segment->getWeight(),
                segment->getService());

        segment->getDest()->removeEdge(segment->getOrigin()->getStation());
        segment->getOrigin()->removeEdge(segment->getDest()->getStation());
    }

    int maxTrains = graph->edmondsKarp(src, dst);

    for (auto segment: segments) {
        graph->addBidirectionalEdge(
                std::get<0>(segment),
                std::get<1>(segment),
                std::get<2>(segment),
                std::get<3>(segment)
        );
    }
    return maxTrains;
}

std::unordered_map<Vertex *, int> capacityOnFailureAllStations(Graph *graph, Edge *segmentToRemove) {
    graph->resetVisited();
    graph->resetFlow();

    std::tuple<std::string, std::string, int, std::string> segment = std::make_tuple(
            segmentToRemove->getOrigin()->getStation().getName(),
            segmentToRemove->getDest()->getStation().getName(),
            segmentToRemove->getWeight(),
            segmentToRemove->getService());

    segmentToRemove->getDest()->removeEdge(segmentToRemove->getOrigin()->getStation());
    segmentToRemove->getOrigin()->removeEdge(segmentToRemove->getDest()->getStation());

    std::unordered_map<Vertex *, int> result_map;

    for (Vertex *vertex: graph->getVertexSet()) {
        int result = graph->edmondsKarpSinkOnly(vertex);
        result_map.insert({vertex, result});
        graph->resetVisited();
        graph->resetFlow();
    }

    graph->addBidirectionalEdge(
            std::get<0>(segment),
            std::get<1>(segment),
            std::get<2>(segment),
            std::get<3>(segment)
    );

    return result_map;
}

std::vector<std::pair<Vertex *, std::pair<int, int>>>
mostAffectedStationsOnSegmentFailure(Graph *graph, Edge *segmentToRemove) {
    std::vector<std::pair<Vertex *, std::pair<int, int>>> result;

    std::unordered_map<Vertex *, int> normalNetworkFlows;

    for (Vertex *vertex: graph->getVertexSet()) {
        normalNetworkFlows.insert({vertex, graph->edmondsKarpSinkOnly(vertex)});
    }

    std::unordered_map<Vertex *, int> reducedNetwork = capacityOnFailureAllStations(graph, segmentToRemove);

    for (auto it: reducedNetwork) {
        auto it2 = normalNetworkFlows.find(it.first);
        if (it2 == normalNetworkFlows.end()) {
            std::cout << "Something went wrong while calculating differences...\n";
            continue;
        }
        int value_before = (*it2).second;
        int value_after = it.second;

        result.push_back({it.first, {value_before, value_after}});
    }
    std::sort(result.begin(), result.end(), [](const auto &it1, const auto &it2) {
        return abs(it1.second.first - it1.second.second) > abs(it2.second.first - it2.second.second);
    });

    return result;
}