/******************************************************************************
 * File        : robot.cpp
 * Owner       : Robert (T5)
 * Status      : STUB - replace each TODO with working code.
 *               Port from Group9FinalDraft1.cpp: the moveRobot lambda and the
 *               "Initialize robot state" block in main().
 ******************************************************************************/
#include "robot.h"
#include "config.h"

Robot::Robot() : pos(), moves(0), distance(0), battery(BATTERY_START) {}

void Robot::reset(Point start) {
    // TODO(Robert): put the robot at start with zero moves and full battery.
    (void)start;
}

void Robot::move(int dr, int dc) {
    // TODO(Robert): step by (dr, dc), add a move, add distance, drain battery.
    (void)dr; (void)dc;
}

bool Robot::hasBattery() const {
    // TODO(Robert): true when there is enough battery for one more move.
    return false;
}

Point Robot::getPosition() const { return pos; }
int   Robot::getMoves() const    { return moves; }
int   Robot::getDistance() const { return distance; }
int   Robot::getBattery() const  { return battery; }
