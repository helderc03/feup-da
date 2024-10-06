#ifndef DATASET_LOADER_H
#define DATASET_LOADER_H

#include <string>
#include "graph/Graph.h"

/**
 * @brief Loads a sample graph (Toy Graph or Extra Fully Connected Graph) from a file
 * @param filePath Path to file
 * @param skipFirstLine Whether to skip the first line of the file
 * @return Pointer to graph or nullptr if file not found
 */
Graph *loadSampleGraph(const std::string& filePath, bool skipFirstLine);

/**
 * @brief Loads a real world graph (larger files) from a file
 * @param edgesFilePath Path to file containing edges
 * @param nodesFilePath Path to file containing nodes
 * @return Pointer to graph or nullptr if file(s) not found
 */
Graph *loadRealWorldGraph(const std::string& nodesFilePath, const std::string& edgesFilePath);

#endif