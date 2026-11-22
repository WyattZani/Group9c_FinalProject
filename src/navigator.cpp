/******************************************************************************
 * File        : navigator.cpp
 * Owner       : Robert (T5, T6)
 * Status      : IN PROGRESS - run() and printResults() done; step() still a stub.
 *               Port from Group9FinalDraft1.cpp: the "Navigation state" and
 *               "Simulation loop" blocks.
 ******************************************************************************/
#include "navigator.h"
#include <iostream>
#include "config.h"
#include "display.h"

using namespace std;

Navigator::Navigator(Grid& g, Point start, Point goal, mt19937& randomGen)
    : grid(g), robot(), A(start), B(goal), rng(randomGen),
      phase(HORIZONTAL), lane(start.r), detouring(false), lastForward(false),
      retreating(false), sideDir(1), outcome(RUNNING) {
    robot.reset(start);
}

// ----------------------------- T5: simulate --------------------------------
// Draw the grid, make one move, pause, and repeat until the run ends.
Outcome Navigator::run() {
    grid.clearVisited();   // erase the path from any earlier run

    render(grid, robot.getPosition(), A, B, robot.getMoves(),
           robot.getDistance(), robot.getBattery(), "Starting");
    pauseMs(STEP_DELAY_MS);

    while (step()) {
        render(grid, robot.getPosition(), A, B, robot.getMoves(),
               robot.getDistance(), robot.getBattery(), "Moving");
        pauseMs(STEP_DELAY_MS);
    }

    string status;
    if (outcome == REACHED) {
        status = "Reached B";
    } else if (outcome == BATTERY_DEAD) {
        status = "Battery dead";
    } else if (outcome == TRAPPED) {
        status = "Trapped";
    } else {
        status = "Stopped";
    }
    render(grid, robot.getPosition(), A, B, robot.getMoves(),
           robot.getDistance(), robot.getBattery(), status);

    return outcome;
}

bool Navigator::step() {
    // TODO(Robert): port one pass of the simulation loop from the draft.
    return false;
}

Outcome Navigator::getOutcome() const {
    return outcome;
}

RunResult Navigator::getResult() const {
    RunResult result;
    result.outcome  = outcome;
    result.moves    = robot.getMoves();
    result.distance = robot.getDistance();
    result.battery  = robot.getBattery();
    return result;
}

// ----------------------------- T6: results ---------------------------------
void printResults(const RunResult& result) {
    cout << "\n   ===== Results =====\n";
    cout << "   Outcome  : ";
    switch (result.outcome) {
        case REACHED:      cout << "Reached point B\n";               break;
        case BATTERY_DEAD: cout << "Battery died before reaching B\n"; break;
        case TRAPPED:      cout << "Trapped - no way forward\n";       break;
        default:           cout << "Still running\n";                  break;
    }
    cout << "   Moves    : " << result.moves    << "\n";
    cout << "   Distance : " << result.distance << " m\n";
    cout << "   Battery  : " << result.battery  << "% left\n";
}