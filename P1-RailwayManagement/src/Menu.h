#ifndef UNTITLED_MENU_H
#define UNTITLED_MENU_H

#include <iomanip>
#include <iostream>
#include "DatasetLoader.h"
#include <vector>
#include "graph/Graph.h"
#include "LineFailures.h"
#include "MinCost.h"
#include <limits>

class Menu {
public:
    /**
     * @brief Auxiliary menu to return a user's choice.
     * Complexity: O(1)
     * @param maxOption User's max option
     * @param minOption User's min option
     * @return
     */
    int auxMenu(int maxOption, int minOption);

    /**
     * @brief Auxiliary function to clear the terminal's screen.
     * Complexity: O(1)
     */
    void clearScreen();

    /**
     * @brief Simple menu used for facilitating the user's interaction with the program.
     * Complexity: O(1)
     * @return User's choice
     */
    int mainMenu();

    /**
    * @brief Simple menu used for facilitating the user's interaction with the program.
    * Complexity: O(1)
    * @return User's choice
    */
    int AboutUsMenu();

    /**
    * @brief Simple menu used for facilitating the user's interaction with the program.
    * Complexity: O(1)
    * @return User's choice
    */
    int bsmMenu();

    /**
    * @brief Simple menu used for facilitating the user's interaction with the program.
    * Complexity: O(1)
    * @return User's choice
    */
    int ocoMenu();

    /**
    * @brief Simple menu used for facilitating the user's interaction with the program.
    * Complexity: O(1)
    * @return User's choice
    */
    int rsMenu();

    void menuController();

    /**
    * @brief Simple menu used for facilitating the user's interaction with the program.
    * Complexity: O(1)
    * @return User's choice
    */
    int dataLoaderMenu();

    void maximumTrainsReducedConnectivityMenu();

    Vertex *getValidStation(std::string displayQuery);

    Edge *getValidSegment(std::string displayQuery);

    bool getBooleanInputFromUser(std::string displayString, bool defaultEnter);

    int getIntegerInputFromUser(std::string displayString);

    void displayTopK(int k, bool useDistricts);
};

#endif //UNTITLED_MENU_H