#ifndef UNTITLED_MENU_H
#define UNTITLED_MENU_H

#include <iomanip>
#include <iostream>
#include "DatasetLoader.h"
#include <vector>
#include "graph/Graph.h"
#include <limits>

class Menu {
public:

/**
 * @brief Allows for user choice in the menus
 *
 * @param maxOption The maximum option value.
 * @param minOption The minimum option value.
 * @return The selected option.
 * @complexity O(1)
 */
    int auxMenu(int maxOption, int minOption);

/**
 * @brief Clears the screen.
 */
    void clearScreen();

/**
 * @brief Displays the main menu and returns the selected option.
 *
 * @return The selected option.
 * @complexity O(1)
 */
    int mainMenu();

/**
 * @brief Displays the about us menu and returns the selected option.
 *
 * @return The selected option.
 * @complexity O(1)
 */
    int aboutUsMenu();

/**
 * @brief Retrieves the list of graph files in the specified folder.
 *
 * @param folderPath The path to the folder containing the graph files.
 * @param[out] fileList The list of graph file names.
 * @complexity O(n), where n is the number of files in the folder.
 */
    void getGraphsInDataset(const std::string& folderPath, std::vector<std::string>& fileList);

/**
 * @brief Displays the menu for choosing the type of graph.
 *
 * @param isBackTrack Flag indicating if it's the backtracking menu.
 * @return The selected type of graph.
 * @complexity O(1)
 */
    int chooseTypeGraph(bool isBackTrack);

/**
 * @brief Retrieves the graph based on the selected type.
 *
 * @param typeGraph The selected type of graph.
 * @return A pointer to the created graph.
 * @complexity O(1)
 */
    Graph* getGraph(const int typeGraph);

/**
 * @brief Controls the main menu flow.
 * @complexity Depends on the invoked functions.
 */
    void menuController();

/**
 * @brief Displays the menu for other heuristics based on the selected graph.
 *
 * @param graph The graph object.
 * @complexity Depends on the invoked heuristic menu functions.
 */
    void otherHeuristicsMenu(Graph* graph);

/**
 * @brief Displays the menu for the simulated annealing heuristic based on the selected graph.
 *
 * @param graph The graph object.
 * @complexity Depends on the invoked simulated annealing menu functions.
 */
    void simulatedAnnealingMenu(Graph* graph);


};

#endif //UNTITLED_MENU_H