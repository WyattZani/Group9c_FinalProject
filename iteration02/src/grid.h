/******************************************************************************
 * File        : grid.h
 * Owner       : Isaiah (T1, T3, T9)
 * Description : The square grid the robot travels on, including obstacle
 *               placement and the route check.
 ******************************************************************************/
#ifndef GRID_H
#define GRID_H

#include <random>
#include <string>
#include "config.h"
#include "point.h"

enum Cell { EMPTY, OBSTACLE, VISITED };

class Grid {
private:
    int  side;
    Cell cells[MAX_SIDE][MAX_SIDE];   // only side x side is used

public:
    Grid();

    // T1 - size and reset
    void setSize(int newSide);
    int  getSize() const;
    void clear();

    // Cell access
    bool inBounds(int r, int c) const;
    bool isFree(int r, int c) const;
    Cell getCell(int r, int c) const;
    void markVisited(int r, int c);
    void clearVisited();

    // T3 - obstacles and route check
    int  placeObstacles(int count, int size, Point A, Point B, std::mt19937& rng);
    bool pathExists(Point A, Point B) const;

    // T9 - save / load (optional)
    bool save(const std::string& filename) const;
    bool load(const std::string& filename);
};

#endif
