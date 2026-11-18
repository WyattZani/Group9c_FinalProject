/******************************************************************************
 * File        : point.h
 * Owner       : Project Leader (T0)
 * Description : A (row, column) position on the grid. 0-based internally.
 ******************************************************************************/
#ifndef POINT_H
#define POINT_H

struct Point {
    int r = -1;
    int c = -1;
};

inline bool samePoint(Point a, Point b) { return a.r == b.r && a.c == b.c; }

inline bool isSet(Point p) { return p.r >= 0 && p.c >= 0; }

#endif
