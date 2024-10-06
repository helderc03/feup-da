#ifndef DA_TSP_GRAPH_H
#define DA_TSP_GRAPH_H

#include <vector>
#include <unordered_map>
#include <queue>
#include <climits>
#include <stack>
#include "VertexEdge.h"
#include "aux_data_structures/MutablePriorityQueue.h"

class Graph {
private:
    std::unordered_map<int, Vertex *> vertexSet;
    double ** matrixGraph;
    bool matrixGraphInitialized = false;
public:
    /**
     * @brief Finds a vertex with a given station name
     * Time Complexity: O(|1|) being |V| the number of vertices in the graph
     * @param station Station name
     * @return Pointer to vertex or nullptr if not found
     */
    Vertex *findVertex(int id) const;

    /**
     * @brief Adds a vertex to the graph
     * Time Complexity: O(1)
     * @param id ID of the vertex to be added
     * @return true if vertex was added, false if it already exists
     */
    bool addVertex(int id);

    /**
     * @brief Adds an edge to the graph
     * Time Complexity: O(1)
     * @param origin Name of the origin station
     * @param dest  Name of the destination station
     * @param weight Capacity of segment
     * @param service Service of the edge to be added
     * @return true if edge was added, false if it already exists
     */
    bool addEdge(int origin, int dest, double weight);

    /**
     * @brief Adds a bidirectional edge to the graph
     * Time Complexity: O(1)
     * @param origin Name of the origin station
     * @param dest Name of the destination station
     * @param weight Capacity of segment
     * @param service Service of the edge to be added
     * @return true if edge was added, false if it already exists
     */
    bool
    addBidirectionalEdge(int origin, int dest, double weight);

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
    std::unordered_map<int, Vertex *> getVertexSet() const;



    /**
     * @brief Resets the visited flag of all vertices
     * Time Complexity: O(|V|) being |V| the number of vertices in the graph
     */
    void resetVisited();

    friend class MutablePriorityQueue<Vertex>;

    /**
     * @brief Solves Travelling Salesman Problem related problems using backtracking
     * Time Complexity: O(n!) being n the number of vertices in the graph
     * @param path vector with the node to start and end the tour
     * @param bestPath initially empty vector that will eventually be filled with the best path
     * @param minCost initially infinity, will eventually be filled with the minimum cost
     * @param cumulatedCost initially 0 and will keep track of the cost so far
     */
    void tsp_backtracking(std::vector<int> &path, std::vector<int> &bestPath, double &minCost, double cumulatedCost); // passar path com vertice de inicio (0), marca-lo como visitado antes de chamar a função, cumulatedCost = 0

    /**
     * @brief Finds the minimum spanning tree of the graph using Prim's algorithm
     * Complexity: O(|E|log|V|) being |E| the number of edges and |V| the number of vertices in the graph
     * @param mst vector of edges that will be filled with the edges of the MST
     */
    void MSTPrim(std::vector<Edge*> &mst);

    /**
     * @brief Determines whether two nodes are connected or not
     * @param id1 first node id
     * @param id2 second node id
     * @return true if connected, false otherwise
     */
    bool connectedNodes(int id1, int id2) const;

    /**
     * @brief Computes the distance of a path passed as argument using the triangular inequality approach
     * Complexity: O(n) being n the number of vertices in the path
     * @param path vector of vertices that represent the path
     * @return distance of the path
     */
    double computePathDistance(std::vector<Vertex*> &path);

    /**
     * @brief Solves the Travelling Salesman Problem using the Triangular Inequality Heuristic, ensuring a 2-approximation
     * Complexity: O(|E|log|V| + |V|^3) being |E| the number of edges and |V| the number of vertices in the graph
     * @return distance of the tour/path
     */
    double triangularInequalityHeuristic();

    /**
     * @brief Transverses a MST given as argument in pre-order
     * Complexity: O(|V|+|E|) being |V| the number of vertices and |E| the number of edges in the graph
     * @param mst the MST
     * @param v the vertex to start the transversal
     * @param path the path to be filled
     */
    void preOrderTraversal(const std::vector<Edge*>& mst, Vertex* v, std::vector<Vertex*> &path);

    /**
     * @brief Uses the tringualar inequality path result and runs 2-opt (switches all 2 pairs of vertices) to find a better result
     * Complexity(|E|log|V| + |V|^3) being |E| the number of edges and |V| the number of vertices
     * 
     * @return double 
     */
    double triangularInequalityHeuristic2Opt();

    /**
     * @brief runs 2-opt algorigthm until no improvements found
     * Complexity(|V|^3) being |V| the number of vertices
     * @param path vector that contains an hamiltonian cycle
     * @param distance sum of path distance
     */
    void twoOpt(std::vector<Vertex*>& path, double& distance);

    /**
     * @brief Uses a base hamiltonian cycle (by linking nodes by ascending id order) and runs 2opt on it
     * Complexity(|V|^3) being |V| the number of vertices
     * 
     * @return double sum of path distance
     */
    double localSearch2Opt();

    /**
     * @brief Uses the triangular inequality algorithm as a base an runs simulatedAnnealing with 2opt switching
     * Complexity(|E|log|V| + |V|^3) being |E| the number of edges and |V| the number of vertices
     * 
     * @param initialTemperature higher makes the algorithm more likely to accept wrong edges througout the algorithm
     * @param steps max number of steps of no-improvement
     * @param cooldownRate multiplier for the temperature MUST be between 0-1
     * @return double sum of least distant path
     */
    double simulatedAnnealing2Opt(double initialTemperature, int steps, double cooldownRate);

    /**
     * @brief Inits the distance matrix according to vertexSet
     * 
     */
    void initDistanceGraph();

    /**
     * @brief Frees the memory occupied by the distance matrix
     * 
     */
    void freeDistanceGraph();

    inline bool isDistanceGraphInitialized(){return matrixGraphInitialized;};
};

#endif //DA_TSP_GRAPH_H
