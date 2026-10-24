/******************************************************************************
 * File        : tasks.h
 * Owner       : Shared - each function belongs to the member listed beside it
 * Description : One function per menu option. main.cpp calls these from its
 *               switch statement. AppState holds everything the tasks share.
 ******************************************************************************/
#ifndef TASKS_H
#define TASKS_H

#include <random>
#include <vector>
#include "grid.h"
#include "navigator.h"
#include "point.h"

struct AppState {
    Grid                   grid;
    int                    obstacleCount = 0;
    Point                  A;                  // unset until T2 runs
    Point                  B;
    bool                   obstaclesReady = false;
    bool                   hasRun         = false;
    RunResult              lastRun;
    std::vector<RunResult> history;            // every run this session (T10)
    std::mt19937           rng{std::random_device{}()};
};

void taskConfigureGrid(AppState& state);    // T1  - Isaiah
void taskSetPoints(AppState& state);        // T2  - Isaiah
void taskGenerateObstacles(AppState& state);// T3  - Isaiah
void taskShowGrid(AppState& state);         // T4  - Wyatt
void taskRunSimulation(AppState& state);    // T5  - Robert
void taskShowResults(AppState& state);      // T6  - Robert
void taskHelp(AppState& state);             // T7  - Wyatt
void taskReset(AppState& state);            // T8  - Wyatt
void taskSaveLoad(AppState& state);         // T9  - Isaiah
void taskHistory(AppState& state);          // T10 - Robert

#endif
