/******************************************************************************
 * File        : robot.h
 * Owner       : Robert (T5)
 * Description : The robot's position, move counter, distance and battery.
 ******************************************************************************/
#ifndef ROBOT_H
#define ROBOT_H

#include "point.h"

class Robot {
private:
    Point pos;
    int   moves;
    int   distance;
    int   battery;

public:
    Robot();

    void  reset(Point start);
    void  move(int dr, int dc);
    bool  hasBattery() const;

    Point getPosition() const;
    int   getMoves() const;
    int   getDistance() const;
    int   getBattery() const;
};

#endif
