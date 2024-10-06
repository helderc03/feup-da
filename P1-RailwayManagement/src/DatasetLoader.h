#ifndef DATASET_LOADER_H
#define DATASET_LOADER_H

#include "graph/Graph.h"
#include "Station.h"

#define DEFAULT_NETWORK_PATH "../dataset/network.csv"
#define DEFAULT_STATIONS_PATH "../dataset/stations.csv"

/**
 * @brief Loads the desired network.csv and stations.csv into Graph.
 * @param networkPath desired network csv file
 * @param stationsPath desired station csv file
 * @return Graph* if successful otherwise returns nullptr.
 */
Graph *loadDataset(std::string networkPath, std::string stationsPath);

#endif