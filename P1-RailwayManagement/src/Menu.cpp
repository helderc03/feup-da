#include "Menu.h"
#include "LineFailures.h"
#include "MinCost.h"
#include <cstdlib>

Graph *graph;
std::string networkPath = DEFAULT_NETWORK_PATH;
std::string stationsPath = DEFAULT_STATIONS_PATH;
bool stayInMenu;

int Menu::auxMenu(int maxOption, int minOption) {
    int op;
    while (true) {
        std::cin >> op;
        if (std::cin.fail() || (op > maxOption || op < minOption)) {
            std::cout << "Please enter a valid integer: ";
            std::cin.clear();
            std::cin.ignore(10000, '\n');
        } else {
            break;
        }
    }
    return op;
}

void Menu::clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    std::cout << "\033[2J\033[1;1H";
#endif
}

int Menu::dataLoaderMenu() {
    clearScreen();
    std::cout << std::endl << "DATA MANAGEMENT MENU \n" << std::endl;
    std::cout << "\t1. Use default data (PT network)" << std::endl;
    std::cout << "\t2. Use new data" << std::endl;
    std::cout << "\t0. Return to main menu" << std::endl;
    std::cout << "\nChoose an option: ";
    return auxMenu(2, 0);

}

int Menu::mainMenu() {
    std::cout << "MAIN MENU\n\n";
    std::cout << "\t1. Basic Service Metrics" << std::endl
              << "\t2. Operation Cost Optimization" << std::endl
              << "\t3. Reliability and Sensitivity to Line Failures" << std::endl
              << "\t4. Change dataset" << std::endl
              << "\t5. About us" << std::endl << std::endl
              << "\t0. Quit" << std::endl;
    std::cout << "----------------------------------------------------------------" << std::endl;
    std::cout << "Choose an option: ";
    return auxMenu(5, 0);
}


int Menu::AboutUsMenu() {
    clearScreen();
    std::cout
            << "\nHelp platform for the management of railway transports created in favor of the Design of Algorithms course\n"
            << std::endl;
    std::cout << "Meet the team: \n\n";
    std::cout << std::left << std::setw(30) << "STUDENT NAME" << std::right << std::setw(18) << "STUDENT NUMBER"
              << std::setw(9) << std::endl << std::endl;
    std::cout << std::left << std::setw(30) << "Hélder Costa" << std::setfill(' ') << std::right << std::setw(18)
              << "202108719" << std::endl;
    std::cout << std::left << std::setw(30) << "Luís Duarte" << std::setfill(' ') << std::right << std::setw(18)
              << " 202108734" << std::endl;
    std::cout << std::left << std::setw(30) << "Luís Jesus" << std::setfill(' ') << std::right << std::setw(18)
              << " 202108683" << std::endl;
    std::cout << std::endl << "0. Return to main menu \n" << std::endl;
    std::cout << "Choose an option: ";
    return auxMenu(0, 0);
}

int Menu::bsmMenu() {
    clearScreen();
    std::cout << std::endl << "BASIC SERVICE METRICS MENU\n" << std::endl;
    std::cout << "\t1. Maximum trains travelling simultaneously between two stations" << std::endl
              << "\t2. Pair of stations requiring the most amount of trains" << std::endl
              << "\t3. Top-k stations requiring larger budgets" << std::endl
              << "\t4. Maximum number of trains that can simultaneously arrive at a station" << std::endl
              << "\t0. Return to main menu" << std::endl << std::endl;
    std::cout << "----------------------------------------------------------------" << std::endl;
    std::cout << "Choose an option: ";
    return auxMenu(4, 0);

}

int Menu::ocoMenu() {
    clearScreen();
    std::cout << std::endl << "OPERATION COST OPTIMIZATION MENU\n";
    std::cout << std::endl << "\t1. Choose source and destination stations \n";
    std::cout << "\t0. Return to main menu\n";
    std::cout << std::endl << "Choose an option: ";
    return auxMenu(1, 0);
}

int Menu::rsMenu() {
    clearScreen();
    std::cout << std::endl << "RELIABILITY AND SENSITIVITY TO FAILURES MENU\n";
    std::cout << std::endl
              << "\t1. Max Number of trains that can simultaneously travel between 2 stations (reduced connectivity network)\n";
    std::cout << "\t2. Most affected stations by segment failure\n";
    std::cout << "\t0. Return to main menu\n";
    std::cout << std::endl << "Choose an option: ";
    return auxMenu(2, 0);
}

void Menu::menuController() {
    int op;
    graph = loadDataset(networkPath, stationsPath);
    clearScreen();
    std::cout << std::endl << "Welcome to the support platform for the management of railway transports!" << std::endl
              << std::endl;
    std::cout << "--------------------RAILWAY ANALYSIS SYSTEM--------------------" << std::endl;
    do {
        int temp;
        op = mainMenu();
        do {
            switch (op) {
                case 1: {
                    int control = bsmMenu();
                    do {
                        switch (control) {
                            case 1: {
                                clearScreen();
                                Vertex *src = getValidStation("origin/source");
                                Vertex *dst = getValidStation("sink/destination");
                                int result = graph->edmondsKarp(src, dst);
                                std::cout << "The max amount of trains travelling simultaneously between "
                                          << src->getStation().getName() << " and " << dst->getStation().getName()
                                          << " is "
                                          << result << "." << std::endl << std::endl;
                                stayInMenu = getBooleanInputFromUser("Do you wish to stay on this menu (y/n)? ", true);
                                if (stayInMenu) {
                                    temp = 1;
                                    control = 0;
                                    break;
                                } else {
                                    clearScreen();
                                    control = 0;
                                    temp = 0;
                                    break;
                                }
                            }

                            case 2: {
                                clearScreen();
                                std::cout << std::endl;
                                std::pair<std::vector<std::pair<Vertex *, Vertex *>>, int> result;
                                std::cout << "\n\t\tCalculating..." << std::endl << std::endl,
                                        result = graph->moreDemandingPairOfStations();
                                clearScreen();
                                std::cout << "The most demanding pair(s) of stations are: " << std::endl << std::endl;
                                for (auto &i: result.first) {
                                    std::cout << "- " << i.first->getStation().getName() << " and "
                                              << i.second->getStation().getName() << std::endl;
                                }
                                std::cout << std::endl << "The amount of trains that can travel simultaneously is "
                                          << result.second << "." << std::endl << std::endl;
                                stayInMenu = getBooleanInputFromUser("Do you wish to stay on this menu (y/n) ?", true);
                                if (stayInMenu) {
                                    temp = 1;
                                    control = 0;
                                    break;
                                } else {
                                    clearScreen();
                                    control = 0;
                                    temp = 0;
                                    break;
                                }

                            }

                            case 3: {
                                clearScreen();
                                std::cout
                                        << "Here we will show you three different criteria that may be subject of analysis to determine the top-k regions that require larger budgets:"
                                        << std::endl << std::endl;
                                std::cout
                                        << "\t1. TOP-K REGIONS WITH HIGHEST WEIGHTED AVERAGE: we use linear scaling to assign a "
                                        << std::endl
                                        << "\tweight to the max-flow that each station can receive. Big Stations (stations that"
                                        << std::endl
                                        << "\treceive more flow) receive a higher weight than small stations (stations that receive "
                                        << std::endl
                                        << "\tless flow). This may give an idea of the regions that require more attention, as"
                                        << std::endl
                                        << "\tthey receive many trains." << std::endl << std::endl;

                                std::cout
                                        << "\t2. TOP-K REGIONS WITH HIGHEST BOTTLENECK: we determine the max Flow that each station"
                                        << std::endl
                                        << "\tcan receive and then we get the station of the region with highest Max Flow. This"
                                        << std::endl
                                        << "\tcan give us an idea of more urbanized/crowded regions as they are more likely to have"
                                        << std::endl
                                        << "\ta 'big central station'. Also, it may be a good region to build other connections as"
                                        << std::endl
                                        << "\tthese stations also may provide a big flow." << std::endl << std::endl;

                                std::cout
                                        << "\t3. TOP-K REGIONS WITH HIGHEST NUMBER OF STATIONS: we count the number of stations in"
                                        << std::endl
                                        << "\teach region and then we get the top-k regions with the highest number of stations."
                                        << std::endl
                                        << "\tThis may give us an idea of the regions that require more attention as they have more"
                                        << std::endl
                                        << "\tstations to maintain." << std::endl << std::endl;
                                int k = getIntegerInputFromUser("Choose the value for K (number of locations): ");
                                std::cout << std::endl;
                                bool useDistrict = getBooleanInputFromUser(
                                        "Do you wish to see the result district-wise or municipality-wise (d or m respectively)? ",
                                        true);
                                std::cout << std::endl;
                                displayTopK(k, useDistrict);
                                stayInMenu = getBooleanInputFromUser(
                                        "Do you wish to stay on this menu and change the settings (y/n)? ", true);
                                if (stayInMenu) {
                                    break;
                                } else {
                                    clearScreen();
                                    control = 0;
                                    temp = 0;
                                    break;
                                }
                            }

                            case 4: {
                                clearScreen();
                                std::cout << std::endl;
                                Vertex *st = getValidStation("desired");
                                int result = graph->edmondsKarpSinkOnly(st);
                                std::cout << "The maximum amount of trains that can arrive at "
                                          << st->getStation().getName()
                                          << " is " << result << std::endl;
                                stayInMenu = getBooleanInputFromUser(
                                        "\nDo you wish to stay on this menu and change the station (y/n)? ", true);
                                if (stayInMenu) {
                                    break;
                                } else {
                                    clearScreen();
                                    control = 0;
                                    temp = 0;
                                    break;
                                }
                            }

                            case 0 : {
                                temp = 0;
                            }

                            default:
                                break;

                        }
                    } while (control != 0);
                    break;
                }

                case 2: {
                    int control = ocoMenu();
                    do {
                        switch (control) {
                            case 1 : {
                                clearScreen();
                                Vertex *src = getValidStation("origin/source");
                                Vertex *dst = getValidStation("sink/destination");
                                std::vector<std::pair<std::vector<Edge *>, int>> answers = getMinCostPaths(graph, src,
                                                                                                           dst);
                                int totalCost = 0;
                                clearScreen();
                                std::cout << "The optimal solution that maintains the max-flow contains "
                                          << answers.size() << " (the cheapest) paths from "
                                          << src->getStation().getName() << " to " << dst->getStation().getName() << "."
                                          << std::endl;

                                for (auto path: answers) {
                                    graph->resetFlow();
                                    graph->resetVisited();
                                    int cost = calculateCostOfPath(path.first);
                                    totalCost += cost * path.second;
                                }

                                std::cout << "Total cost for the company: " << totalCost << "€" << std::endl;
                                bool seePaths = getBooleanInputFromUser("\nDo you wish to see the paths (y/n)? ", true);
                                if (seePaths) {
                                    clearScreen();
                                    int countPath = 1;
                                    for (auto path: answers) {
                                        graph->resetFlow();
                                        graph->resetVisited();
                                        int cost = calculateCostOfPath(path.first);
                                        std::cout << "This path should be done with " << path.second << " trains.\n";
                                        std::cout << "The total cost per train is: " << cost << "€.\n\n";
                                        std::cout << "The total cost is: " << cost * path.second << "€.\n\n";
                                        std::cout << "\t" << path.first[0]->getDest()->getStation().getName() << "\n";
                                        std::cout << "\t^\n";
                                        std::cout << "\t|" << "\033[1;31m Service: " << path.first[0]->getService()
                                                  << "\033[0m" << std::endl;
                                        std::cout << "\t|\n";
                                        for (auto it = path.first.begin(); it != (--path.first.end()); it++) {
                                            std::cout << "\t" << (*it)->getOrigin()->getStation().getName()
                                                      << std::endl;
                                            std::cout << "\t^\n";
                                            std::cout << "\t|" << "\033[1;31m Service: " << (*it)->getService()
                                                      << "\033[0m" << std::endl;
                                            std::cout << "\t|\n";
                                        }
                                        std::cout << "\t" << path.first[path.first.size() -
                                                                        1]->getOrigin()->getStation().getName()
                                                  << std::endl << std::endl;
                                        if (countPath < answers.size()) {
                                            std::cout << "\nYou've just seen " << countPath << " of " << answers.size()
                                                      << " paths. ";
                                            bool next = getBooleanInputFromUser(
                                                    "Do you wish to see the next one (y/n)? ", true);
                                            if (!next) {
                                                clearScreen();
                                                break;
                                            } else clearScreen();
                                        }
                                        countPath++;
                                    }
                                }
                                stayInMenu = getBooleanInputFromUser(
                                        "Do you wish to stay on this menu and change the stations (y/n)? ", true);
                                if (stayInMenu) {
                                    break;
                                } else {
                                    clearScreen();
                                    control = 0;
                                    temp = 0;
                                    break;
                                }
                            }
                            case 0 : {
                                temp = 0;
                            }
                        }
                    } while (control != 0);
                    break;
                }

                case 3: {
                    int control = rsMenu();
                    do {
                        switch (control) {
                            case 1 : {
                                clearScreen();
                                maximumTrainsReducedConnectivityMenu();
                                stayInMenu = getBooleanInputFromUser("\nDo you wish to continue on this menu (y/n)? ",
                                                                     true);
                                if (stayInMenu) {
                                    break;
                                } else {
                                    clearScreen();
                                    control = 0;
                                    temp = 0;
                                    break;
                                }
                            }

                            case 2: {
                                clearScreen();
                                int k = getIntegerInputFromUser("Choose the number of entries to display: ");
                                clearScreen();
                                Edge *segment = getValidSegment("desired segment");
                                std::vector<std::pair<Vertex *, std::pair<int, int>>> result;
                                result = mostAffectedStationsOnSegmentFailure(graph, segment);
                                clearScreen();
                                std::cout << "The most affected stations on the removal of the chosen segment are: "
                                          << std::endl
                                          << std::endl;
                                for (int i = 0; i < k; i++) {
                                    std::cout << "- " << result[i].first->getStation().getName()
                                              << " with an initial maximum amount of trains of "
                                              << result[i].second.first
                                              << " and a final maximum amount of trains of " << result[i].second.second
                                              << std::endl;
                                }

                                stayInMenu = getBooleanInputFromUser(
                                        "\nDo you wish to stay on this menu and change the segment (y/n)? ", true);
                                if (stayInMenu) {
                                    break;
                                } else {
                                    control = 0;
                                    temp = 0;
                                    break;
                                }

                            }

                            case 0 : {
                                temp = 0;
                            }

                            default:
                                break;
                        }
                    } while (control != 0);
                    break;
                }

                case 4: {
                    int control = dataLoaderMenu();
                    do {
                        switch (control) {
                            case 1: {
                                networkPath = DEFAULT_NETWORK_PATH;
                                stationsPath = DEFAULT_STATIONS_PATH;
                                graph = loadDataset(networkPath, stationsPath);
                                control = 0;
                                temp = 0;
                                break;
                            }

                            case 2: {
                                while (true) {
                                    std::cout << "\nInsert the paths for new data: " << "\n"
                                              << "Path for network file: ";
                                    std::cin >> networkPath;
                                    std::cout << std::endl;

                                    if (networkPath == "q" || networkPath == "Q") {
                                        control = 1;
                                        break;
                                    }

                                    std::cout << "Path for the stations file:";
                                    std::cin >> stationsPath;
                                    std::cout << std::endl;

                                    if (stationsPath == "q" || stationsPath == "Q") {
                                        control = 1;
                                        break;
                                    }

                                    graph = loadDataset(networkPath, stationsPath);

                                    if (graph != nullptr) break;

                                    std::cout
                                            << "Make sure you entered the correct paths, write 'q' to revert to default data and go to main menu\n";
                                }
                                control = 0;
                                temp = 0;
                                break;
                            }

                            case 0: {
                                temp = 0;
                                break;
                            }

                        }

                    } while (control != 0);
                    break;
                }

                case 5: {
                    temp = AboutUsMenu();
                    break;
                }


                case 0: {
                    temp = 0;
                    break;
                }

                default:
                    break;

            }

        } while (temp != 0);
        clearScreen();

    } while (op != 0);
    std::cout << "\n";
    std::cout << "Thank you for using our platform!" << std::endl;
}

void Menu::maximumTrainsReducedConnectivityMenu() {
    std::cout << "======================================================" << std::endl << std::endl;
    Vertex *src = getValidStation("source");
    Vertex *dst = getValidStation("destination");
    std::vector<Edge *> segmentsToExclude;
    clearScreen();
    while (true) {
        Edge *edge = getValidSegment("BROKEN SEGMENT (the segment that will be removed from the network)");
        segmentsToExclude.push_back(edge);
        clearScreen();
        if (!getBooleanInputFromUser("Do you wish to insert more segments (y/n): ", false)) break;
    }
    int normalFlow = graph->edmondsKarp(src, dst);
    int result = maximumTrainsReducedConnectivity(graph, segmentsToExclude, src, dst);
    std::cout << "\nRESULTS: " << std::endl;
    std::cout << "- With reduced connectivity: \t Max-Flow:" << result << "\n";
    std::cout << "- Normal Network: \t Max-Flow:" << normalFlow << "\n";
}

Vertex *Menu::getValidStation(std::string displayQuery) {
    Vertex *v;
    do {
        std::cout << "Please select a " << displayQuery << " station: ";
        std::string station;
        do {
            std::getline(std::cin, station);
        } while (station.empty());
        v = graph->findVertex(station);
        std::cout << std::endl;
        if (v == nullptr) std::cout << "Invalid station name... please try again..\n\n";
    } while (v == nullptr);
    return v;
}

Edge *Menu::getValidSegment(std::string displayQuery) {
    std::cout << "PLEASE SELECT THE " << displayQuery << std::endl << std::endl;
    while (true) {
        Vertex *src = getValidStation("segment start");
        Vertex *dst = getValidStation("segment end");
        for (auto edge: src->getEdges()) {
            if (edge->getDest() == dst) {
                std::cout << "========================\n\n";
                return edge;
            }
        }
        clearScreen();
        std::cout << src->getStation().getName() << " is not adjacent to " << dst->getStation().getName()
                  << "... try again\n";

    }
}

bool Menu::getBooleanInputFromUser(std::string displayString, bool defaultEnter) {
    while (true) {
        std::cin.clear();
        std::cin.sync();
        std::cout << displayString;
        char choice = 0;
        std::cin >> choice;
        if (choice == 0 || choice == 10) {
            return defaultEnter;
        } else if (choice == 'y' || choice == 'Y') {
            return true;
        } else if (choice == 'n' || choice == 'N') {
            return false;
        } else if (choice == 'd' || choice == 'D') {
            return true;
        } else if (choice == 'm' || choice == 'M') {
            return false;
        }

        std::cout << "\n Option not valid... try again...\n";
    }
}

int Menu::getIntegerInputFromUser(std::string displayString) {
    while (true) {
        std::cin.clear();
        int r = 0;
        std::cout << displayString;
        if ((std::cin >> r).good() && r < graph->getNumVertex()) {
            return r;
        }
        std::cout << "Invalid input... only a number is accepted... try again...\n";
    }
}

void Menu::displayTopK(int k, bool useDistricts) {
    std::vector<std::pair<std::string, float>> municipalitiesHighestAverage;
    std::vector<std::pair<std::string, int>> municipalitiesHighestBottleneck;
    std::vector<std::pair<std::string, int>> municipalitiesMoreStations;
    graph->findTopK(municipalitiesHighestAverage, municipalitiesHighestBottleneck, municipalitiesMoreStations, k,
                    useDistricts);


    const int tableWidth = 80;

    std::cout << std::setfill('=') << std::right << std::setw(tableWidth) << "" << std::endl;
    std::cout << std::setfill(' ') << std::left << std::setw(42) << "Municipality"
              << std::setfill(' ') << std::right << std::setw(20) << "Weighted Average Of Stations' Max-Flow"
              << std::endl;
    std::cout << std::setfill('-') << std::setw(tableWidth) << "" << std::endl;
    for (auto el: municipalitiesHighestAverage) {
        std::cout << std::setw(50) << std::setfill(' ') << std::left << el.first
                  << std::setw(30) << std::setfill(' ') << std::right << std::fixed << std::setprecision(2) << el.second
                  << std::endl;
    }
    std::cout << std::setfill('=') << std::setw(tableWidth) << "" << std::endl;

    std::cout << std::endl;

    std::cout << std::setfill(' ') << std::right << std::setw(30) << "" << std::endl;
    std::cout << std::setfill('=') << std::setw(tableWidth) << "" << std::endl;
    std::cout << std::setfill(' ') << std::left << std::setw(50) << "Municipality"
              << std::setfill(' ') << std::right << std::setw(30) << "Highest Bottleneck" << std::endl;
    std::cout << std::setfill('-') << std::setw(tableWidth) << "" << std::endl;
    for (auto el: municipalitiesHighestBottleneck) {
        std::cout << std::setfill(' ') << std::left << std::setw(50) << el.first
                  << std::setfill(' ') << std::right << std::setw(30) << el.second << std::endl;
    }
    std::cout << std::setfill('=') << std::setw(tableWidth) << "" << std::endl;

    std::cout << std::endl;

    std::cout << std::setfill(' ') << std::right << std::setw(30) << "" << std::endl;
    std::cout << std::setfill('=') << std::setw(tableWidth) << "" << std::endl;
    std::cout << std::setfill(' ') << std::left << std::setw(45) << "Municipality"
              << std::setfill(' ') << std::right << std::setw(30) << "Number Of Stations" << std::endl;
    std::cout << std::setfill('-') << std::setw(tableWidth) << "" << std::endl;
    for (auto el: municipalitiesMoreStations) {
        std::cout << std::setfill(' ') << std::left << std::setw(50) << el.first
                  << std::setfill(' ') << std::right << std::setw(30) << el.second << std::endl;
    }
    std::cout << std::setfill('=') << std::setw(tableWidth) << "" << std::endl;
}

