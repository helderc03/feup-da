#ifndef LINE_FAILURES_H
#define LINE_FAILURES_H

#include "graph/Graph.h"
#include <unordered_map>

/**
 * @brief Computes the maximum number of trains that can be transported from src to dst after removing segments of the network
 * Time Complexity: O(n) being n the number of segments to remove
 * @param graph The graph representing the network
 * @param segmentsToRemove The segments to remove
 * @param src A pointer to the source vertex
 * @param dst A pointer to the destination vertex
 * @return The maximum number of trains that can be transported from src to dst after removing segments of the network
 */
int maximumTrainsReducedConnectivity(Graph *graph, std::vector<Edge *> segmentsToRemove, Vertex *src, Vertex *dst);

/**
 * @brief Computes the maximum number of trains that every station can receive after removing segments of the network using Edmonds-Karp algorithm
 * Time Complexity: O(|V|²|E|²) being |V| the number of vertices and |E| the number of edges
 * @param graph The graph representing the network
 * @param segmentToRemove The segment to remove
 * @return A map with the maximum number of trains that every station can receive after removing segments of the network
 */
std::unordered_map<Vertex *, int> capacityOnFailureAllStations(Graph *graph, Edge *segmentToRemove);

/**
 * @brief Computes the most affected stations on segment failure, i.e. the stations that lose the most trains after removing a segment
 * Time Complexity: O(|V|²|E|²) being |V| the number of vertices and |E| the number of edges
 * @param graph The graph representing the network
 * @param segmentToRemove The segment to remove
 * @return A vector of pairs of pairs of pointers to vertices and pairs of integers, where the first pair is the vertex and the second pair is the number of trains that the station can receive before and after removing the segment
 */
std::vector<std::pair<Vertex *, std::pair<int, int>>>
mostAffectedStationsOnSegmentFailure(Graph *graph, Edge *segmentToRemove);

#endif