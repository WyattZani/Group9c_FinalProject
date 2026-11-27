/******************************************************************************
 * File        : navigator.cpp
 * Owner       : Robert (T5, T6)
 * Status      : DONE - navigation logic ported from Group9FinalDraft1.cpp.
 ******************************************************************************/
#include "navigator.h"
#include <cstdlib>
#include <iostream>
#include "config.h"
#include "display.h"

using namespace std;

// T5, Robert: name of a one-cell move, used in status messages.
static string dirName(int dr, int dc) {
    if (dr == -1) return "up";
    if (dr ==  1) return "down";
    if (dc == -1) return "left";
    return "right";
}

// T5, Robert: set up the navigation state and put the robot at A.
Navigator::Navigator(Grid& g, Point start, Point goal, mt19937& randomGen)
    : grid(g), robot(), A(start), B(goal), rng(randomGen),
      phase(HORIZONTAL), lane(start.r), detouring(false), lastForward(false),
      retreating(false), sideDir(1), outcome(RUNNING),
      status("Robot powered on at point A") {
    robot.reset(start);
}

// T5, Robert: true if (r, c) is on the grid and not an obstacle.
bool Navigator::canMove(int r, int c) const {
    return grid.inBounds(r, c) && grid.isFree(r, c);
}

// T5, Robert: mark the current cell as travelled, then move the robot.
void Navigator::moveRobot(int dr, int dc) {
    Point pos = robot.getPosition();
    grid.markVisited(pos.r, pos.c);
    robot.move(dr, dc);
}

// ----------------------------- T5: simulate --------------------------------
// T5, Robert: draw the grid, make one move, pause, and repeat until the run ends.
Outcome Navigator::run() {
    grid.clearVisited();   // erase the path from any earlier run

    render(grid, robot.getPosition(), A, B, robot.getMoves(),
           robot.getDistance(), robot.getBattery(), status);
    pauseMs(STEP_DELAY_MS);

    while (step()) {
        render(grid, robot.getPosition(), A, B, robot.getMoves(),
               robot.getDistance(), robot.getBattery(), status);
        pauseMs(STEP_DELAY_MS);
    }

    if (outcome == REACHED) {
        status = "Reached point B";
    } else if (outcome == BATTERY_DEAD) {
        status = "Battery dead - failed to reach point B";
    } else if (outcome == TRAPPED) {
        status = "No possible moves - robot is trapped";
    }
    render(grid, robot.getPosition(), A, B, robot.getMoves(),
           robot.getDistance(), robot.getBattery(), status);

    return outcome;
}

// T5, Robert: make one move toward B. Returns false when the run is over.
bool Navigator::step() {
    if (outcome != RUNNING) {
        return false;
    }

    Point pos = robot.getPosition();
    if (samePoint(pos, B)) {
        outcome = REACHED;
        return false;
    }

    // Switch axis once lined up with B
    string note;
    if (phase == HORIZONTAL && pos.c == B.c) {
        phase = VERTICAL;
        lane = pos.c;
        detouring = false;
        lastForward = false;
        retreating = false;
        note = "Lined up with B - switching to vertical. ";
    } else if (phase == VERTICAL && pos.r == B.r) {
        phase = HORIZONTAL;
        lane = pos.r;
        detouring = false;
        lastForward = false;
        retreating = false;
        note = "Re-aligning horizontally. ";
    }

    // Battery check before every move
    if (!robot.hasBattery()) {
        outcome = BATTERY_DEAD;
        return false;
    }

    // Forward direction (toward B on the current axis) and the perpendicular axis
    int fdr, fdc, pdr, pdc, perpOff, perpToGoal;
    if (phase == HORIZONTAL) {
        fdr = 0;
        fdc = (B.c > pos.c) ? 1 : -1;
        pdr = 1;
        pdc = 0;
        perpOff = pos.r - lane;
        perpToGoal = B.r - pos.r;
    } else {
        fdr = (B.r > pos.r) ? 1 : -1;
        fdc = 0;
        pdr = 0;
        pdc = 1;
        perpOff = pos.c - lane;
        perpToGoal = B.c - pos.c;
    }

    // Normal travel: check the path ahead first
    if (!detouring) {
        if (canMove(pos.r + fdr, pos.c + fdc)) {
            moveRobot(fdr, fdc);
            lastForward = true;
            status = note + "Path clear - moving " + dirName(fdr, fdc);
            return true;
        }
        detouring = true;
        lastForward = false;
        if (perpToGoal > 0) {
            sideDir = 1;
        } else if (perpToGoal < 0) {
            sideDir = -1;
        } else {
            uniform_int_distribution<int> coin(0, 1);
            sideDir = coin(rng) ? 1 : -1;
        }
        note += "Obstacle ahead! ";
    }

    // Detour: work around the obstacle, then return to the original path
    int backDir = (perpOff > 0) ? -1 : 1;   // direction back toward the lane
    int sr = sideDir * pdr;
    int sc = sideDir * pdc;

    if (perpOff != 0 && lastForward &&
        canMove(pos.r + backDir * pdr, pos.c + backDir * pdc)) {
        moveRobot(backDir * pdr, backDir * pdc);
        lastForward = false;
        if (abs(perpOff) == 1) {
            detouring = false;
            status = note + "Obstacle cleared - back on original path";
        } else {
            status = note + "Stepping " + dirName(backDir * pdr, backDir * pdc)
                   + " toward original path";
        }
    } else if (!retreating && canMove(pos.r + fdr, pos.c + fdc)) {
        moveRobot(fdr, fdc);
        lastForward = true;
        if (perpOff == 0) {
            detouring = false;
            status = note + "Path clear - moving " + dirName(fdr, fdc);
        } else {
            status = note + "Moving " + dirName(fdr, fdc) + " alongside obstacle";
        }
    } else if (canMove(pos.r + sr, pos.c + sc)) {
        moveRobot(sr, sc);
        lastForward = false;
        retreating = false;
        status = note + "Blocked - side-stepping " + dirName(sr, sc);
    } else if (canMove(pos.r - sr, pos.c - sc)) {
        sideDir = -sideDir;
        moveRobot(-sr, -sc);
        lastForward = false;
        retreating = false;
        status = note + "That side is blocked - side-stepping " + dirName(-sr, -sc);
    } else if (canMove(pos.r - fdr, pos.c - fdc)) {
        moveRobot(-fdr, -fdc);
        lastForward = false;
        retreating = true;
        status = note + "Dead end - backing up " + dirName(-fdr, -fdc);
    } else {
        outcome = TRAPPED;
        return false;
    }
    return true;
}

// T5, Robert: how the run ended (RUNNING until it is over).
Outcome Navigator::getOutcome() const {
    return outcome;
}

// T5, Robert: summary of the run for T6 results and T10 history.
RunResult Navigator::getResult() const {
    RunResult result;
    result.outcome  = outcome;
    result.moves    = robot.getMoves();
    result.distance = robot.getDistance();
    result.battery  = robot.getBattery();
    return result;
}

// ----------------------------- T6: results ---------------------------------
// T6, Robert: print the outcome, moves, distance and battery of a run.
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