#ifndef MIN_COST_H
#define MIN_COST_H

#include "graph/Graph.h"


/**
 * @brief Calculates the paths that cost less to the company, considering the capacity of the network
 * Time complexity: O (|V||E|^2) where |V| is the number of vertices and |E| the number of edges
 * @param graph Pointer to the original Graph object
 * @param src Pointer to the source of the path
 * @param dst Pointer to the destination of the path
 * @return std::vector<std::pair<std::vector<Edge*>, int>> a list of possible paths, that contains the list representation of the path and the trains used
 */
std::vector<std::pair<std::vector<Edge *>, int>> getMinCostPaths(Graph *graph, Vertex *src, Vertex *dst);

/**
 * @brief Sets the path variable on each vertex to make an equal representation of a path.
 * It's a compatibility layer between minCost paths and other paths.
 * Time Complexity: O(|E|) where |E| is the number of edges
 *  
 * @param path List of edges that is a representaion of a path 
 */
void reconstructPath(std::vector<Edge *> path);

/**
 * @brief Calculates the cost of path, using a list representation of a path.
 * Time Complexity: O(|E|) where |E| is the number of edges
 * @param path List of edges that is a representaion of a path 
 * @return int Total cost
 */
int calculateCostOfPath(std::vector<Edge *> path);

#endif