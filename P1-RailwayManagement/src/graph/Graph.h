#ifndef DA_RAILWAYMANAGEMENT_GRAPH_H
#define DA_RAILWAYMANAGEMENT_GRAPH_H

#include <vector>
#include <queue>
#include <climits>
#include "VertexEdge.h"

class Graph {
private:
    std::vector<Vertex *> vertexSet;
public:
    /**
     * @brief Finds a vertex with a given station name
     * Time Complexity: O(|V|) being |V| the number of vertices in the graph
     * @param station Station name
     * @return Pointer to vertex or nullptr if not found
     */
    Vertex *findVertex(const std::string &station) const;

    /**
     * @brief Finds a vertex with a given station name
     * Time Complexity: O(|V|) being |V| the number of vertices in the graph
     * @param station Station name
     * @return Pointer to vertex or nullptr if not found
     */
    bool addVertex(const Station &station);

    /**
     * @brief Adds an edge to the graph
     * Time Complexity: O(|V|) being |V| the number of vertices in the graph
     * @param origin Name of the origin station
     * @param dest  Name of the destination station
     * @param weight Capacity of segment
     * @param service Service of the edge to be added
     * @return true if edge was added, false if it already exists
     */
    bool addEdge(const std::string &origin, const std::string &dest, int weight, const std::string &service);

    /**
     * @brief Adds a bidirectional edge to the graph
     * Time Complexity: O(|V|) being |V| the number of vertices in the graph
     * @param origin Name of the origin station
     * @param dest Name of the destination station
     * @param weight Capacity of segment
     * @param service Service of the edge to be added
     * @return true if edge was added, false if it already exists
     */
    bool
    addBidirectionalEdge(const std::string &origin, const std::string &dest, int weight, const std::string &service);

    /**
     * @brief Gets the number of vertices in the graph
     * Time Complexity: O(1)
     * @return Number of vertices
     */
    int getNumVertex() const;

    /**
     * @brief Gets the edges of the graph
     * Time Complexity: O(1)
     * @return Vector of edges
     */
    std::vector<Vertex *> getVertexSet() const;

    /**
     * @brief Performs a Depth-First Search on the graph
     * Time Complexity: O(|V| + |E|) being |V| the number of vertices and |E| the number of edges in the graph
     * @param source Pointer to the source vertex
     */
    void DFS(Vertex *source);

    /**
     * @brief Resets the visited flag of all vertices
     * Time Complexity: O(|V|) being |V| the number of vertices in the graph
     */
    void resetVisited();

    /**
     * @brief Resets the flow of all edges in the graph
     * Time Complexity: O(|V||E|) being |E| the number of edges in the graph
     */
    void resetFlow();

    /**
     * @brief Helper function for Edmonds-Karp algorithm that marks vertices as visited and set paths if there is residual capacity
     * Time Complexity: O(1)
     * @param q Queue of vertices
     * @param e Edge to be considered
     * @param w Vertex to be considered
     * @param residual Residual capacity of the edge
     */
    void testAndVisit(std::queue<Vertex *> &q, Edge *e, Vertex *w, float residual);

    /**
     * @brief Finds the minimum residual capacity along a path
     * Time Complexity: O(|V|) being |V| the number of vertices in the path
     * @param source Pointer to the source vertex
     * @param dest Pointer to the destination vertex
     * @return Minimum residual capacity
     */
    int findMinResidualAlongPath(Vertex *source, Vertex *dest);

    /**
     * @brief Performs BFS to find an augmenting path in the graph from source to destination
     * Time Complexity: O(|V| + |E|) being |V| the number of vertices and |E| the number of edges in the graph
     * @param source Pointer to the source vertex
     * @param dest Pointer to the destination vertex
     * @return true if there is an augmenting path, false otherwise
     */
    bool findAugmentingPath(Vertex *source, Vertex *dest);

    /**
     * @briefs Augments the flow along a path
     * Time Complexity: O(|V|) being |V| the number of vertices in the path
     * @param source Pointer to the source vertexo
     * @param dest Pointer to the destination vertex
     * @param minResidual Minimum residual capacity of the path
     */
    void augmentFlowAlongPath(Vertex *source, Vertex *dest, int minResidual);

    /**
     * @brief Finds the maximum flow in the graph from source to destination using Edmonds-Karp algorithm
     * Time Complexity: O(|V||E|^2) being |V| the number of vertices and |E| the number of edges in the graph
     * @param source Pointer to the source vertex
     * @param dest Pointer to the destination vertex
     * @return Maximum flow in the graph
     */
    int edmondsKarp(Vertex *source, Vertex *dest);

    /**
     * @brief Finds the maximum flow that can reach the destination vertex using Edmonds-Karp algorithm
     * Time Complexity: O(|V||E|^2) being |V| the number of vertices and |E| the number of edges in the graph
     * @param dest Pointer to the destination vertex
     * @return Maximum flow that can reach the destination vertex
     */
    int edmondsKarpSinkOnly(Vertex *dest);

    /**
     * @brief Finds the most demanding pair of stations in the graph in terms of maximum flow
     * Time Complexity: O(|V|³|E|²) being |V| the number of vertices and |E| the number of edges in the graph
     * @return A pair containing a vector with the most demanding pairs of stations and the maximum flow
     */
    std::pair<std::vector<std::pair<Vertex *, Vertex *>>, int> moreDemandingPairOfStations();

    /**
     * @brief Gives Top K stations in terms of maximum flow weighted mean, highest bottleneck and more stations for further analysis
     * Time Complexity: O(|V||E|^2) being |V| the number of vertices and |E| the number of edges in the graph
     * @param maxFlowWeightedAverage Vector to store the top K stations in terms of maximum flow weighted mean
     * @param highestBottleneck Vector to store the top K stations in terms of highest bottleneck
     * @param moreStations Vector to store the top K stations in terms of number of stations
     * @param k
     * @param useDistricts
     */
    void findTopK(std::vector<std::pair<std::string, float>> &maxFlowWeightedAverage,
                  std::vector<std::pair<std::string, int>> &highestBottleneck,
                  std::vector<std::pair<std::string, int>> &moreStations,
                  int k, bool useDistricts);
};


#endif //DA_RAILWAYMANAGEMENT_GRAPH_H
