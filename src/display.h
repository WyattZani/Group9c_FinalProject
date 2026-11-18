/******************************************************************************
 * File        : display.h
 * Owner       : Wyatt (T4, T7)
 * Description : Everything that draws to the console.
 ******************************************************************************/
#ifndef DISPLAY_H
#define DISPLAY_H

#include <string>
#include "grid.h"
#include "point.h"

void setupConsole();
void clearScreen();
void pauseMs(int ms);

// T7 - help and legend
void printHeader();
void printLegend();

// T4 - grid display
std::string cellSymbol(const Grid& grid, int r, int c, Point robot, Point A, Point B);
void render(const Grid& grid, Point robot, Point A, Point B, int moves,
            int distance, int battery, const std::string& status);

#endif
