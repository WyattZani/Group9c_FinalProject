/******************************************************************************
 * File        : grid.cpp
 * Owner       : Isaiah (T1, T3, T9)
 * Status      : T1 and T3 DONE (ported from Group9FinalDraft1.cpp into the
 *               Grid class). T9 save / load is still a STUB.
 ******************************************************************************/
#include "grid.h"
#include <iostream>
#include <queue>

using namespace std;

Grid::Grid() : side(0) {
    clear();
}

// ------------------------------ T1: size -----------------------------------
// Stores the side length. 0 means "not configured" (used by reset).
void Grid::setSize(int newSide) {
    if (newSide == 0 || (newSide >= MIN_SIDE && newSide <= MAX_SIDE))
        side = newSide;
}

int Grid::getSize() const {
    return side;
}

// Sets every cell back to EMPTY.
void Grid::clear() {
    for (int r = 0; r < MAX_SIDE; r++)
        for (int c = 0; c < MAX_SIDE; c++)
            cells[r][c] = EMPTY;
}

// ---------------------------- Cell access ----------------------------------
// True when (r, c) is inside the side x side grid.
bool Grid::inBounds(int r, int c) const {
    return r >= 0 && r < side && c >= 0 && c < side;
}

// True when (r, c) is in bounds and not an obstacle.
bool Grid::isFree(int r, int c) const {
    return inBounds(r, c) && cells[r][c] != OBSTACLE;
}

// Returns the cell at (r, c). Anything outside the grid counts as an obstacle.
Cell Grid::getCell(int r, int c) const {
    if (!inBounds(r, c))
        return OBSTACLE;
    return cells[r][c];
}

// Marks (r, c) as part of the robot's path. Never paints over an obstacle.
void Grid::markVisited(int r, int c) {
    if (isFree(r, c))
        cells[r][c] = VISITED;
}

// Turns every VISITED cell back into EMPTY, keeping the obstacles.
void Grid::clearVisited() {
    for (int r = 0; r < MAX_SIDE; r++)
        for (int c = 0; c < MAX_SIDE; c++)
            if (cells[r][c] == VISITED)
                cells[r][c] = EMPTY;
}

// ------------------------- T3: obstacles -----------------------------------
// Places obstacles Battleship-style (each one is `size` cells in a straight
// horizontal or vertical line), never covering A or B. Returns how many fit.
int Grid::placeObstacles(int count, int size, Point A, Point B, mt19937& rng) {
    clear();
    if (side == 0 || size < 1 || size > side)
        return 0;

    uniform_int_distribution<int> coin(0, 1);
    int placed = 0;

    for (int i = 0; i < count; i++) {
        for (int tries = 0; tries < 200; tries++) {
            bool horizontal = coin(rng) == 1;
            int maxR = horizontal ? side - 1 : side - size;
            int maxC = horizontal ? side - size : side - 1;
            int r0 = uniform_int_distribution<int>(0, maxR)(rng);
            int c0 = uniform_int_distribution<int>(0, maxC)(rng);

            bool ok = true;
            for (int k = 0; k < size && ok; k++) {
                int r = r0 + (horizontal ? 0 : k);
                int c = c0 + (horizontal ? k : 0);
                Point p{r, c};
                if (cells[r][c] != EMPTY || samePoint(p, A) || samePoint(p, B))
                    ok = false;
            }
            if (ok) {
                for (int k = 0; k < size; k++) {
                    int r = r0 + (horizontal ? 0 : k);
                    int c = c0 + (horizontal ? k : 0);
                    cells[r][c] = OBSTACLE;
                }
                placed++;
                break;
            }
        }
    }
    return placed;
}

// Breadth-first search: confirms a route from A to B exists at all.
bool Grid::pathExists(Point A, Point B) const {
    if (!isFree(A.r, A.c) || !isFree(B.r, B.c))
        return false;

    bool seen[MAX_SIDE][MAX_SIDE] = {};
    const int dr[4] = {-1, 1, 0, 0};
    const int dc[4] = {0, 0, -1, 1};
    queue<Point> q;
    q.push(A);
    seen[A.r][A.c] = true;

    while (!q.empty()) {
        Point p = q.front();
        q.pop();
        if (samePoint(p, B))
            return true;
        for (int d = 0; d < 4; d++) {
            int nr = p.r + dr[d];
            int nc = p.c + dc[d];
            if (isFree(nr, nc) && !seen[nr][nc]) {
                seen[nr][nc] = true;
                q.push(Point{nr, nc});
            }
        }
    }
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
