/******************************************************************************
 * File        : tasks.cpp
 * Owner       : Shared - only edit the functions in YOUR section
 * Status      : All STUBS. Each prints "not implemented yet" until its owner
 *               replaces it with working code.
 ******************************************************************************/
#include "tasks.h"
#include <iostream>
#include "display.h"
#include "input.h"

using namespace std;

static void notImplemented(const string& taskName) {
    cout << "\n   " << taskName << " - not implemented yet\n";
}

// =============================== ISAIAH ====================================
// T1: ask for grid side and obstacle count, then size the grid.
void taskConfigureGrid(AppState& state) {
    notImplemented("T1 Configure grid");
    (void)state;
}

// T2: ask for points A and B (must be different).
void taskSetPoints(AppState& state) {
    notImplemented("T2 Set start and goal");
    (void)state;
}

// T3: place obstacles, retrying until a route from A to B exists.
void taskGenerateObstacles(AppState& state) {
    notImplemented("T3 Generate obstacles");
    (void)state;
}

// T9 (optional): save the current map to a file or load one.
void taskSaveLoad(AppState& state) {
    notImplemented("T9 Save / load map");
    (void)state;
}

// =============================== WYATT =====================================
// T4: draw the grid with A, B and obstacles.
void taskShowGrid(AppState& state) {
    if (state.grid.getSize() == 0) {
        cout << "\n   Configure the grid first (option 1).\n";
        return;
    }
    // the robot hasn't moved yet, so it is drawn at A
    render(state.grid, state.A, state.A, state.B, 0, 0, BATTERY_START, "Map preview");
}


// T7: program description and legend.
void taskHelp(AppState& state) {
    notImplemented("T7 Help and legend");
    (void)state;
}

// T8: clear the grid, points and last run.
void taskReset(AppState& state) {
    notImplemented("T8 Reset / new map");
    (void)state;
}

// =============================== ROBERT ====================================
// T5: build a Navigator and run the animated simulation.
void taskRunSimulation(AppState& state) {
    notImplemented("T5 Run simulation");
    (void)state;
}

// T6: print the result of the last run.
void taskShowResults(AppState& state) {
    notImplemented("T6 Show results");
    (void)state;
}

// T10 (optional): success rate and averages across all runs.
void taskHistory(AppState& state) {
    notImplemented("T10 Run history");
    (void)state;
}
