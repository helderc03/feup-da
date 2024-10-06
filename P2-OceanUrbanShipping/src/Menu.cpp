#include "Menu.h"
#include <cstdlib>
#include <chrono>
#include <algorithm>
#include <iostream>
#include <filesystem>

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

int Menu::mainMenu() {
    clearScreen();
    std::cout << "\nMAIN MENU\n\n";
    std::cout << "\t1. Backtracking Algorithm" << std::endl
              << "\t2. Triangular Approximation Heuristic" << std::endl
              << "\t3. Other Heuristics" << std::endl
              << "\t4. About Us" << std::endl << std::endl
              << "\t0. Quit" << std::endl;
    std::cout << "----------------------------------------------------------------" << std::endl;
    std::cout << "Choose an option: ";
    return auxMenu(4, 0);
}

int Menu::aboutUsMenu() {
    clearScreen();
    std::cout
            << "\nHelp platform for the management of shipping and deliveries created in favor of the Design of Algorithms course\n"
            << std::endl;
    std::cout << "Meet the team: \n\n";
    std::cout << std::left << std::setw(30) << "STUDENT NAME" << std::right << std::setw(18) << "STUDENT NUMBER"
              << std::setw(9) << std::endl << std::endl;
    std::cout << std::left << std::setw(30) << "Hélder Costa" << std::setfill(' ') << std::right << std::setw(18)
              << " 202108719" << std::endl;
    std::cout << std::left << std::setw(30) << "Luís Duarte" << std::setfill(' ') << std::right << std::setw(18)
              << " 202108734" << std::endl;
    std::cout << std::left << std::setw(30) << "Luís Jesus" << std::setfill(' ') << std::right << std::setw(18)
              << " 202108683" << std::endl;
    std::cout << std::endl << "0. Return to main menu \n" << std::endl;
    std::cout << "Choose an option: ";
    return auxMenu(0, 0);
}

void Menu::getGraphsInDataset(const std::string& folderPath, std::vector<std::string>& fileList) {
    fileList.clear();
    std::filesystem::path path(folderPath);

    if (!std::filesystem::exists(path) || !std::filesystem::is_directory(path)) {
        std::cout << "Invalid folder path: " << folderPath << std::endl;
        return;
    }

    for (const auto& entry : std::filesystem::directory_iterator(path)) {
        if (std::filesystem::is_directory(entry.path())) {
            std::string folderName = entry.path().filename().string();
            if (folderName.find("graph") == 0) {
                fileList.push_back(folderName);
            } else {
                getGraphsInDataset(entry.path().string(), fileList);
            }
        } else {
            std::string extension = entry.path().extension().string();
            if (extension == ".csv") {
                std::string fileName = entry.path().filename().string();
                fileList.push_back(fileName);
            }
        }
    }
}

int Menu::chooseTypeGraph(bool isBackTrack){
    clearScreen();
    std::cout << std::endl << "Which type of graph would you like to use ?\n";
    if (isBackTrack) {
        std::cout << "Warning: Take into consideration that bigger graphs may take up to days to run\n";
    }
    std::cout << "----------------------------------------------------------------\n" << std::endl;
    std::cout << "\t1. Toy Graph\n";
    std::cout << "\t2. Extra Fully Connected Graph\n";
    std::cout << "\t3. Real World Graph\n";
    std::cout << "\nChoose an option: ";
    return auxMenu(3, 1);

}

Graph* Menu::getGraph(const int typeGraph){
    clearScreen();
    std::vector<std::string> fileList;
    std::string toyGraphs, EFCGraphs, RWGraphs;
    toyGraphs = "../dataset/Toy-Graphs";
    EFCGraphs = "../dataset/Extra_Fully_Connected_Graphs";
    RWGraphs = "../dataset/Real-world Graphs";
    int fileCount = 0;
    std::cout << std::endl << "Now choose the exact graph you want to use.\n";
    std::cout << "----------------------------------------------------------------\n" << std::endl;
    if (typeGraph == 1){
        getGraphsInDataset(toyGraphs, fileList);
        for (const auto& file : fileList) {
            std::cout << "\t" << fileCount << ". " << file << std::endl;
            fileCount++;
        }
        std::cout << "\nChoose an option: ";
        int choice = auxMenu(fileCount,0);
        std::string path = toyGraphs + "/" + fileList[choice];
        return loadSampleGraph(path, true);

    }
    else if (typeGraph == 2){
        getGraphsInDataset(EFCGraphs, fileList);
        for (const auto& file : fileList) {
            std::cout << "\t" << fileCount << ". " << file << std::endl;
            fileCount++;
        }
        std::cout << "\nChoose an option: ";
        int choice = auxMenu(fileCount,0);
        std::string path = EFCGraphs + "/" + fileList[choice];
        return loadSampleGraph(path, false);
    }
    else if (typeGraph == 3){
        getGraphsInDataset(RWGraphs, fileList);
        for (const auto& file : fileList) {
            std::cout << "\t" << fileCount << ". " << file << std::endl;
            fileCount++;
        }
        std::cout << "\nChoose an option: ";
        int choice = auxMenu(fileCount,0);
        std::string pathNodes = RWGraphs + "/" + fileList[choice] + "/nodes.csv";
        std::string pathEdges = RWGraphs + "/" + fileList[choice] + "/edges.csv";
        return loadRealWorldGraph(pathNodes, pathEdges);
    }

    return nullptr;
}

void Menu::menuController() {
    int op;
    //load dataset
    clearScreen();
    std::cout << std::endl << "Welcome to the support platform for the management of ocean shipping and urban deliveries!" << std::endl
              << std::endl;
    std::cout << "--------------------SHIPPING AND DELIVERIES ANALYSIS SYSTEM--------------------" << std::endl;
    do {
        int temp;
        op = mainMenu();
        do {
            switch (op) {

                case 1:{
                    int typeGraph = chooseTypeGraph(true);
                    Graph* graph = getGraph(typeGraph);
                    temp = 0;
                    graph->resetVisited();

                    std::vector<int> path, bestPath;
                    double minCost = std::numeric_limits<double>::max();
                    double cumulatedCost = 0.0;
                    path.push_back(0);
                    graph->findVertex(0)->setVisited(true);
                    auto start = std::chrono::high_resolution_clock::now();
                    graph->tsp_backtracking(path, bestPath, minCost, cumulatedCost);
                    auto end = std::chrono::high_resolution_clock::now();
                    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end-start);
                    clearScreen();
                    std::cout << "\n----------------------------------------------------------------\n";
                    std::cout << "\t\t RESULTS ";
                    std::cout << "\n----------------------------------------------------------------\n" << std::endl;
                    std::cout << std::endl << "For the graph in question the best path was:\n";

                    for (size_t i = 0; i < bestPath.size(); i++) {
                        std::cout << bestPath[i];
                        if (i != bestPath.size() - 1) {
                            std::cout << " -> ";
                        }
                    }
                    std::cout << " -> " << bestPath[0] << std::endl;

                    std::cout << std::endl << "The total distance was: " << std::fixed << std::setprecision(1) << minCost << "m\n";
                    std::cout << "\nAnd the algorithm ran in: " << std::fixed << std::setprecision(4) << (double) duration.count() / 1000 << "ms\n";
                    std::cout << "\n Press enter to continue...\n";
                    std::cin.clear();
                    std::cin.sync();
                    std::cin.get();
                    getchar();
                    break;
                }

                case 2:{
                    int typeGraph = chooseTypeGraph(false);
                    Graph* graph = getGraph(typeGraph);
                    temp = 0;
                    graph->resetVisited();
                    auto start = std::chrono::high_resolution_clock::now();
                    double distance = graph->triangularInequalityHeuristic();
                    auto end = std::chrono::high_resolution_clock::now();
                    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end-start);
                    clearScreen();
                    std::cout << "\n----------------------------------------------------------------\n";
                    std::cout << "\t\t RESULTS ";
                    std::cout << "\n----------------------------------------------------------------\n" << std::endl;
                    std::cout << std::endl << "The total distance was: " << std::fixed << std::setprecision(1) << distance << "m\n";
                    std::cout << "\nAnd the algorithm ran in: " << std::fixed << std::setprecision(4) << (double) duration.count() / 1000 << "ms\n";
                    std::cout << "\n Press enter to continue...\n";
                    std::cin.clear();
                    std::cin.sync();
                    std::cin.get();
                    getchar();
                    break;
                }

                case 3:{
                    int typeGraph = chooseTypeGraph(false);
                    Graph* graph = getGraph(typeGraph);
                    temp = 0;
                    otherHeuristicsMenu(graph);
                    break;
                }

                case 4: {
                    temp = aboutUsMenu();
                    break;
                }
                default:
                    break;
            }
        }while (temp != 0);
    }while (op != 0);
    std::cout << "\n";
    std::cout << "Thank you for using our platform!" << std::endl;
}


void Menu::simulatedAnnealingMenu(Graph * graph){
    clearScreen();
    std::cout << "\nChoose an option...\n\n";
    std::cout << "\t1. Default options\n";
    std::cout << "\t2. Custom options\n\n";
    std::cout << "Enter your selection: ";
    int selection;
    if(!(std::cin >> selection).good()){
        simulatedAnnealingMenu(graph);
    }
    int initialTemperature;
    int numberOfSteps;
    double cooldownRatio;
    if(selection == 1){
        initialTemperature = 5000;
        numberOfSteps = 5*graph->getNumVertex()*graph->getNumVertex();
        cooldownRatio = 0.95;

    } else if(selection == 2){
        std::cout << "\nEnter initial temperature: ";
        std::cin >> initialTemperature;
        std::cout << "\nEnter max number of steps without improvement: ";
        std::cin >> numberOfSteps;
        std::cout << "\nEnter cooldown ratio (0-1 double): ";
        std::cin >> cooldownRatio;
    } else {
        simulatedAnnealingMenu(graph);
    }

    if(cooldownRatio < 0 || cooldownRatio > 1){
        simulatedAnnealingMenu(graph);
        return;
    }
    auto start = std::chrono::high_resolution_clock::now();
    double distance = graph->simulatedAnnealing2Opt(initialTemperature, numberOfSteps, cooldownRatio);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end-start);

    clearScreen();
    std::cout << "\n----------------------------------------------------------------\n";
    std::cout << "\t\t RESULTS ";
    std::cout << "\n----------------------------------------------------------------\n" << std::endl;
    
    std::cout << "\t\t Simulated annealing\n\n";
    std::cout << "Distance: " << std::fixed << std::setprecision(4) << distance << "m\n";
    std::cout << "Run time: " << std::fixed << std::setprecision(4) << (double) duration.count() / 1000 << "ms\n";
    std::cout << "\nPress enter to continue...\n";

    std::cin.clear();
    std::cin.sync();
    std::cin.get();
    getchar();
    otherHeuristicsMenu(graph);

}

void Menu::otherHeuristicsMenu(Graph * graph) {
    if(!graph->isDistanceGraphInitialized()){
        graph->initDistanceGraph();
    }
    clearScreen();
        std::cout << "\n\t\tOther Heuristics\n\n";
    std::cout << "1. Local search 2-opt only" << std::endl
              << "2. Triangular Approximation Heuristic with 2-opt optimization" << std::endl
              << "3. Simulated Annealing using 2-opt" << std::endl
              << "4. Do All and choose minimum" << std::endl
              << "0. Go back" << std::endl;
    std::cout << "----------------------------------------------------------------" << std::endl;
    std::cout << "Choose an option: ";
    int selection;
    if((std::cin >> selection).fail()){
        otherHeuristicsMenu(graph);
    }
    if(selection == 1){
        auto start = std::chrono::high_resolution_clock::now();
        double distance = graph->localSearch2Opt();
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end-start);
        
        clearScreen();
        std::cout << "\n----------------------------------------------------------------\n";
        std::cout << "\t\t RESULTS ";
        std::cout << "\n----------------------------------------------------------------\n" << std::endl;
        std::cout << "\t\t Local search 2-opt only\n\n";
        std::cout << "Distance: " << std::fixed << std::setprecision(4) << distance << "m\n";
        std::cout << "Run time: " << std::fixed << std::setprecision(4) << (double) duration.count() / 1000 << "ms\n";
        std::cout << "\n Press enter to continue...\n";
        std::cin.clear();
        std::cin.sync();
        std::cin.get();
        getchar();


    }
    if(selection == 2){
        auto start = std::chrono::high_resolution_clock::now();
        double distance = graph->triangularInequalityHeuristic2Opt();
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end-start);
        
        clearScreen();
        std::cout << "\n----------------------------------------------------------------\n";
        std::cout << "\t\t RESULTS ";
        std::cout << "\n----------------------------------------------------------------\n" << std::endl;
        std::cout << "\t\t Triangular approximation heuristic with 2-opt optimization\n\n";
        std::cout << "Distance: " << std::fixed << std::setprecision(4) << distance << "m\n";
        std::cout << "Run time: " << std::fixed << std::setprecision(4) << (double) duration.count() / 1000 << "ms\n";
        std::cout << "\n Press enter to continue...\n";
        std::cin.clear();
        std::cin.sync();
        std::cin.get();
        getchar();


    }
    if(selection == 3){
        simulatedAnnealingMenu(graph);
    }
    if(selection == 4){
        std::vector<double> distances;
        auto start = std::chrono::high_resolution_clock::now(); 
        distances.push_back(graph->localSearch2Opt());
        distances.push_back(graph->triangularInequalityHeuristic2Opt());
        distances.push_back(graph->simulatedAnnealing2Opt(5000, 5*graph->getNumVertex()*graph->getNumVertex(), 0.95));
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end-start);
        clearScreen();
        std::cout << "\n----------------------------------------------------------------\n";
        std::cout << "\t\t RESULTS ";
        std::cout << "\n----------------------------------------------------------------\n" << std::endl;
        std::cout << "\t\t All alternative methods\n\n";
        std::cout << "Distance: " << std::fixed << std::setprecision(4) << *std::min_element(distances.begin(), distances.end()) << "m\n";
        std::cout << "Run time: " << std::fixed << std::setprecision(4) << (double) duration.count() / 1000 << "ms\n";
        std::cout << "\n Press enter to continue...\n";
        std::cin.clear();
        std::cin.sync();
        std::cin.get();
        getchar();


    }
    if(selection == 0){
        return;
    }
    otherHeuristicsMenu(graph);

}
