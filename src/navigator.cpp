/******************************************************************************
 * File        : navigator.cpp
 * Owner       : Robert (T5, T6)
 * Status      : STUB - replace each TODO with working code.
 *               Port from Group9FinalDraft1.cpp: the "Navigation state" and
 *               "Simulation loop" blocks, and the "Results" block in main().
 ******************************************************************************/
#include "navigator.h"
#include <iostream>

using namespace std;

Navigator::Navigator(Grid& g, Point start, Point goal, mt19937& randomGen)
    : grid(g), robot(), A(start), B(goal), rng(randomGen),
      phase(HORIZONTAL), lane(start.r), detouring(false), lastForward(false),
      retreating(false), sideDir(1), outcome(RUNNING) {
    robot.reset(start);
}

// ----------------------------- T5: simulate --------------------------------
Outcome Navigator::run() {
    // TODO(Robert): call render() and step() until the run ends.
    cout << "   [Navigator::run] not implemented yet\n";
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
    // TODO(Robert): port the "Results" block from the draft.
    cout << "   [printResults] not implemented yet\n";
    (void)result;
}
