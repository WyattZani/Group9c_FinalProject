/******************************************************************************
 * File        : grid.cpp
 * Owner       : Isaiah (T1, T3, T9)
 * Status      : STUB - replace each TODO with working code.
 *               Port from Group9FinalDraft1.cpp: clearGrid(), inBounds(),
 *               isFree(), placeObstacles(), pathExists().
 ******************************************************************************/
#include "grid.h"
#include <iostream>

using namespace std;

Grid::Grid() : side(0) {
    clear();
}

// ------------------------------ T1: size -----------------------------------
void Grid::setSize(int newSide) {
    // TODO(Isaiah): store the side length.
    cout << "   [Grid::setSize] not implemented yet\n";
    (void)newSide;
}

int Grid::getSize() const {
    return side;
}

void Grid::clear() {
    // TODO(Isaiah): set every cell to EMPTY.
}

// ---------------------------- Cell access ----------------------------------
bool Grid::inBounds(int r, int c) const {
    // TODO(Isaiah): true when (r, c) is inside the side x side grid.
    (void)r; (void)c;
    return false;
}

bool Grid::isFree(int r, int c) const {
    // TODO(Isaiah): in bounds and not an obstacle.
    (void)r; (void)c;
    return false;
}

Cell Grid::getCell(int r, int c) const {
    // TODO(Isaiah): return the cell at (r, c).
    (void)r; (void)c;
    return EMPTY;
}

void Grid::markVisited(int r, int c) {
    // TODO(Isaiah): mark (r, c) as VISITED.
    (void)r; (void)c;
}

void Grid::clearVisited() {
    // TODO(Isaiah): turn every VISITED cell back into EMPTY (keeps obstacles).
}

// ------------------------- T3: obstacles -----------------------------------
// Places obstacles Battleship-style; returns how many were placed.
int Grid::placeObstacles(int count, int size, Point A, Point B, mt19937& rng) {
    // TODO(Isaiah): port placeObstacles() from the draft.
    cout << "   [Grid::placeObstacles] not implemented yet\n";
    (void)count; (void)size; (void)A; (void)B; (void)rng;
    return 0;
}

// Breadth-first search: confirms a route from A to B exists at all.
bool Grid::pathExists(Point A, Point B) const {
    // TODO(Isaiah): port pathExists() from the draft.
    (void)A; (void)B;
    return false;
}

// ---------------------- T9: save / load (optional) -------------------------
bool Grid::save(const string& filename) const {
    // TODO(Isaiah): write side and cells to a text file.
    cout << "   [Grid::save] not implemented yet\n";
    (void)filename;
    return false;
}

bool Grid::load(const string& filename) {
    // TODO(Isaiah): read side and cells from a text file.
    cout << "   [Grid::load] not implemented yet\n";
    (void)filename;
    return false;
}
