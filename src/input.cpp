/******************************************************************************
 * File        : input.cpp
 * Owner       : Project Leader (readInt, T0) / Isaiah (readPoint, T2)
 * Status      : readInt() and readPoint() are DONE.
 ******************************************************************************/
#include "input.h"
#include <iostream>
#include <sstream>
#include <cstdlib>

using namespace std;

// Reads a whole number in [lo, hi]; re-prompts on bad input.
int readInt(const string& prompt, int lo, int hi) {
    while (true) {
        cout << prompt << " [" << lo << "-" << hi << "]: ";
        string line;
        if (!getline(cin, line)) exit(1);
        stringstream ss(line);
        int value;
        char extra;
        if (ss >> value && !(ss >> extra) && value >= lo && value <= hi)
            return value;
        cout << "   Invalid input - enter a whole number from "
             << lo << " to " << hi << ".\n";
    }
}

// ----------------------------- T2: points ----------------------------------
// Reads a point as 1-based row and column (what the user sees on screen),
// and stores it 0-based (how the grid is indexed).
Point readPoint(const string& name, int side) {
    cout << "\n Enter coordinates for point " << name << ":\n";
    Point p;
    p.r = readInt("   Row   ", 1, side) - 1;
    p.c = readInt("   Column", 1, side) - 1;
    return p;
}
