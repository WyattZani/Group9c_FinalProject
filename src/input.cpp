/******************************************************************************
 * File        : input.cpp
 * Owner       : Project Leader (readInt, T0) / Isaiah (readPoint, T2)
 * Status      : readInt() is DONE (the menu needs it). readPoint() is a STUB.
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
Point readPoint(const string& name, int side) {
    // TODO(Isaiah): port readPoint() from the draft using readInt().
    cout << "   [readPoint " << name << "] not implemented yet\n";
    (void)side;
    return Point{};
}
