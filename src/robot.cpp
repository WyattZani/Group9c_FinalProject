/******************************************************************************
 * File        : robot.cpp
 * Owner       : Robert (T5)
 * Status      : DONE - position, move counter, distance and battery.
 ******************************************************************************/
#include "robot.h"
#include "config.h"

Robot::Robot() : pos(), moves(0), distance(0), battery(BATTERY_START) {}

// Put the robot at start with zero moves and a full battery.
void Robot::reset(Point start) {
    pos      = start;
    moves    = 0;
    distance = 0;
    battery  = BATTERY_START;
}

// Step by (dr, dc), count the move, add distance, and drain the battery.
void Robot::move(int dr, int dc) {
    pos.r += dr;
    pos.c += dc;
    moves++;
    distance += METERS_PER_MOVE;
    battery  -= BATTERY_PER_MOVE;
    if (battery < 0) {
        battery = 0;
    }
}

// True when there is enough battery left for one more move.
bool Robot::hasBattery() const {
    return battery >= BATTERY_PER_MOVE;
}

Point Robot::getPosition() const { return pos; }
int   Robot::getMoves() const    { return moves; }
int   Robot::getDistance() const { return distance; }
int   Robot::getBattery() const  { return battery; }