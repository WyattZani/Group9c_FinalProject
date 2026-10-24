/******************************************************************************
 * File        : navigator.h
 * Owner       : Robert (T5, T6)
 * Description : Drives the robot from A to B: horizontal first, then
 *               vertical, detouring around obstacles.
 ******************************************************************************/
#ifndef NAVIGATOR_H
#define NAVIGATOR_H

#include <random>
#include "grid.h"
#include "robot.h"

enum Phase   { HORIZONTAL, VERTICAL };
enum Outcome { RUNNING, REACHED, BATTERY_DEAD, TRAPPED };

// Summary of one finished run (used by T6 results and T10 history).
struct RunResult {
    Outcome outcome  = RUNNING;
    int     moves    = 0;
    int     distance = 0;
    int     battery  = 0;
};

class Navigator {
private:
    Grid&         grid;
    Robot         robot;
    Point         A;
    Point         B;
    std::mt19937& rng;

    Phase   phase;
    int     lane;          // the original path (row or column) being followed
    bool    detouring;
    bool    lastForward;   // last move was along the travel direction
    bool    retreating;    // backing out of a dead end
    int     sideDir;       // +1 / -1 perpendicular side-step direction
    Outcome outcome;

public:
    Navigator(Grid& g, Point start, Point goal, std::mt19937& randomGen);

    Outcome   run();          // T5 - loop step() until the run ends
    bool      step();         // T5 - one move; false when the run is over
    Outcome   getOutcome() const;
    RunResult getResult() const;
};

void printResults(const RunResult& result);   // T6

#endif
