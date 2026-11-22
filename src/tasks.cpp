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
// A new grid size makes the old points and obstacles meaningless, so they
// are cleared and have to be set again (options 2 and 3).
void taskConfigureGrid(AppState& state) {
    int side = readInt(" Enter the grid side length", MIN_SIDE, MAX_SIDE);
    int obstacleSize = side / 2;
    cout << "   Each obstacle will be " << obstacleSize << " cell(s) long.\n";
    int count = readInt(" Enter the number of obstacles", 0, side - 1);

    state.grid.clear();
    state.grid.setSize(side);
    state.obstacleCount  = count;
    state.A              = Point{};
    state.B              = Point{};
    state.obstaclesReady = false;
    state.hasRun         = false;

    cout << "\n   Grid set to " << side << " x " << side << " with "
         << count << " obstacle(s). Next, set points A and B (option 2).\n";
}

// T2: ask for points A and B (must be different).
// New points clear any old obstacles, since an obstacle could now sit on A or B.
void taskSetPoints(AppState& state) {
    int side = state.grid.getSize();
    if (side == 0) {
        cout << "\n   Configure the grid first (option 1).\n";
        return;
    }

    state.A = readPoint("A (start)", side);
    while (true) {
        state.B = readPoint("B (goal)", side);
        if (!samePoint(state.A, state.B))
            break;
        cout << "   Point B must be different from point A.\n";
    }

    state.grid.clear();
    state.obstaclesReady = false;
    state.hasRun         = false;

    cout << "\n   A = (" << state.A.r + 1 << ", " << state.A.c + 1 << "), B = ("
         << state.B.r + 1 << ", " << state.B.c + 1
         << "). Next, generate obstacles (option 3).\n";
}

// T3: place obstacles, retrying until a route from A to B exists.
// If every attempt fails, it tries again with one fewer obstacle, so a route
// is always guaranteed (0 obstacles always works).
void taskGenerateObstacles(AppState& state) {
    int side = state.grid.getSize();
    if (side == 0) {
        cout << "\n   Configure the grid first (option 1).\n";
        return;
    }
    if (!isSet(state.A) || !isSet(state.B)) {
        cout << "\n   Set points A and B first (option 2).\n";
        return;
    }

    int obstacleSize = side / 2;
    int target = state.obstacleCount;
    bool valid = false;

    while (!valid) {
        for (int attempt = 0; attempt < LAYOUT_ATTEMPTS && !valid; attempt++) {
            int placed = state.grid.placeObstacles(target, obstacleSize,
                                                   state.A, state.B, state.rng);
            valid = (placed == target) && state.grid.pathExists(state.A, state.B);
        }
        if (!valid)
            target--;
    }

    state.obstaclesReady = true;
    state.hasRun         = false;

    if (target < state.obstacleCount)
        cout << "\n   Could not fit all " << state.obstacleCount
             << " obstacles with a route open, so " << target << " were placed.\n";
    else
        cout << "\n   " << target << " obstacle(s) placed, and a route from A to B is open.\n";
    cout << "   Show the grid (option 4) or run the simulation (option 5).\n";
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
    if (state.history.empty()) {
        cout << "\n   No runs yet. Run the simulation first (option 5).\n";
        return;
    }

    int runs = state.history.size();
    int reached = 0;
    int totalMoves = 0;
    int totalBattery = 0;

    for (int i = 0; i < runs; i++) {
        if (state.history[i].outcome == REACHED) {
            reached++;
        }
        totalMoves += state.history[i].moves;
        totalBattery += state.history[i].battery;
    }

    cout << "\n   ===== Run history =====\n";
    cout << "   Runs         : " << runs << "\n";
    cout << "   Reached B    : " << reached << " ("
         << 100.0 * reached / runs << "%)\n";
    cout << "   Avg moves    : " << (double)totalMoves / runs << "\n";
    cout << "   Avg battery  : " << (double)totalBattery / runs << "% left\n";
}