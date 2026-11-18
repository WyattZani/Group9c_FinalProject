/******************************************************************************
 * Program     : Robot Path Navigator
 * Authors     : Group 9C - Wyatt Zani, Robert, Isaiah
 * Language    : C++17
 * File        : main.cpp
 * Owner       : Project Leader (T0, T11)
 * Description : Menu loop. Each option runs exactly one task from tasks.cpp.
 *
 * Build       : g++ -std=c++17 -Wall src/main.cpp src/tasks.cpp src/grid.cpp
 *                   src/robot.cpp src/navigator.cpp src/display.cpp
 *                   src/input.cpp -o robot_navigator
 * Run         : ./robot_navigator        (Windows: robot_navigator.exe)
 ******************************************************************************/
#include <iostream>
#include "display.h"
#include "input.h"
#include "tasks.h"

using namespace std;

void printMenu() {
    cout << "\n------------------------- MAIN MENU -------------------------\n";
    cout << "  1. Configure grid (size and obstacle count)\n";
    cout << "  2. Set start A and goal B\n";
    cout << "  3. Generate obstacles\n";
    cout << "  4. Show grid\n";
    cout << "  5. Run simulation\n";
    cout << "  6. Show results of last run\n";
    cout << "  7. Help and legend\n";
    cout << "  8. Reset / new map\n";
    cout << "  9. Save / load map\n";
    cout << " 10. Run history\n";
    cout << "  0. Exit\n";
    cout << "-------------------------------------------------------------\n";
}

int main() {
    setupConsole();
    printHeader();

    AppState state;
    bool running = true;

    while (running) {
        printMenu();
        int choice = readInt(" Choose an option", 0, 10);

        switch (choice) {
            case 1:  taskConfigureGrid(state);     break;
            case 2:  taskSetPoints(state);         break;
            case 3:  taskGenerateObstacles(state); break;
            case 4:  taskShowGrid(state);          break;
            case 5:  taskRunSimulation(state);     break;
            case 6:  taskShowResults(state);       break;
            case 7:  taskHelp(state);              break;
            case 8:  taskReset(state);             break;
            case 9:  taskSaveLoad(state);          break;
            case 10: taskHistory(state);           break;
            case 0:
                cout << "\n Goodbye!\n";
                running = false;
                break;
        }
    }
    return 0;
}
