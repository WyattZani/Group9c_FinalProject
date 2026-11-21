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
    if (!isSet(state.A) || !isSet(state.B)) {
        cout << "\n   Set points A and B first (option 2).\n";
        return;
    }
    // the robot hasn't moved yet, so it is drawn at A
    render(state.grid, state.A, state.A, state.B, 0, 0, BATTERY_START, "Map preview");
}


// T7: program description and legend.
void taskHelp(AppState& state) {
    (void)state;
    printHeader();
    printLegend();
    cout << " Typical run: 1 -> 2 -> 3 -> 4 -> 5 -> 6\n";
}

// T8: clear the grid, points and last run.
void taskReset(AppState& state) {
    state.grid.clear();
    state.grid.setSize(0);
    state.obstacleCount  = 0;
    state.A              = Point{};
    state.B              = Point{};
    state.obstaclesReady = false;
    state.hasRun         = false;
    state.lastRun        = RunResult{};
    cout << "\n   Map, points and last run cleared. Run history kept.\n";
}

// =============================== ROBERT ====================================
// T5: build a Navigator and run the animated simulation.
void taskRunSimulation(AppState& state) {
    if (state.grid.getSize() == 0) {
        cout << "\n   Configure the grid first (option 1).\n";
        return;
    }
    if (!isSet(state.A) || !isSet(state.B)) {
        cout << "\n   Set points A and B first (option 2).\n";
        return;
    }
    if (!state.obstaclesReady) {
        cout << "\n   Generate obstacles first (option 3).\n";
        return;
    }

    Navigator nav(state.grid, state.A, state.B, state.rng);
    nav.run();

    state.lastRun = nav.getResult();
    state.hasRun  = true;
    state.history.push_back(state.lastRun);   // saved for T10

    printResults(state.lastRun);
}

// T6: print the result of the last run.
void taskShowResults(AppState& state) {
    if (!state.hasRun) {
        cout << "\n   No run yet. Run the simulation first (option 5).\n";
        return;
    }
    printResults(state.lastRun);
}

// T10 (optional): success rate and averages across all runs.
void taskHistory(AppState& state) {
    notImplemented("T10 Run history");
    (void)state;
}
