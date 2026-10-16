/******************************************************************************
 * Program     : Robot Path Navigator
 * Author      : Wyatt Zani
 * Language    : C++17
 * Description : Simulates a battery-powered robot navigating a square grid
 *               from point A to point B. The grid contains Battleship-style
 *               obstacles placed randomly on a horizontal or vertical axis.
 *               The robot first moves horizontally to line up with B, then
 *               vertically, side-stepping around obstacles and returning to
 *               its original path once clear. Each move costs 1 meter and
 *               2% battery (50 moves total). If the battery runs out before
 *               reaching B, the robot dies.
 *
 * Build       : g++ -std=c++17 -O2 robot_navigator.cpp -o robot_navigator
 ******************************************************************************/
 
#define NOMINMAX
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <random>
#include <queue>
#include <thread>
#include <chrono>
#include <cstdlib>
#ifdef _WIN32
  #include <windows.h>
  #ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
    #define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
  #endif
#endif
 
using namespace std;
 
// ----------------------------- Configuration -------------------------------
const bool USE_UNICODE = true;   // set false if your console shows garbled symbols
 
const int MIN_SIDE         = 3;
const int MAX_SIDE         = 10;
const int BATTERY_START    = 100;
const int BATTERY_PER_MOVE = 2;
const int METERS_PER_MOVE  = 1;
const int STEP_DELAY_MS    = 500;
const int LAYOUT_ATTEMPTS  = 500;
 
// Display symbols
const string SYM_EMPTY    = USE_UNICODE ? "\u207B" : ".";   // ⁻ open space
const string SYM_OBSTACLE = USE_UNICODE ? "\u25A7" : "#";   // ▧ obstacle
const string SYM_PATH     = USE_UNICODE ? "\u223A" : "*";   // ∺ path travelled
const string SYM_ROBOT    = "R";
const string SYM_START    = "A";
const string SYM_GOAL     = "B";
 
// ------------------------------- Types -------------------------------------
enum Cell    { EMPTY, OBSTACLE, VISITED };
enum Phase   { HORIZONTAL, VERTICAL };
enum Outcome { RUNNING, REACHED, BATTERY_DEAD, TRAPPED };
 
struct Point {
    int r;
    int c;
};
 
// ------------------------------- Globals -----------------------------------
Cell grid[MAX_SIDE][MAX_SIDE];   // 2D array; only side x side is used
int  side = 0;
 
// --------------------------- Utility functions -----------------------------
bool samePoint(Point a, Point b) { return a.r == b.r && a.c == b.c; }
 
bool inBounds(int r, int c) { return r >= 0 && r < side && c >= 0 && c < side; }
 
bool isFree(int r, int c) { return inBounds(r, c) && grid[r][c] != OBSTACLE; }
 
string dirName(int dr, int dc) {
    if (dr == -1) return "up";
    if (dr ==  1) return "down";
    if (dc == -1) return "left";
    return "right";
}
 
void setupConsole() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;
    if (GetConsoleMode(h, &mode))
        SetConsoleMode(h, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
#endif
}
 
void clearScreen() { cout << "\033[2J\033[H" << flush; }
 
void pauseMs(int ms) { this_thread::sleep_for(chrono::milliseconds(ms)); }
 
// ---------------------------- Header / legend ------------------------------
void printHeader() {
    cout << "============================================================\n";
    cout << "                   ROBOT PATH NAVIGATOR\n";
    cout << "============================================================\n";
    cout << " Program  : Robot Path Navigator (obstacle avoidance sim)\n";
    cout << " Author   : Wyatt Zani\n";
    cout << "------------------------------------------------------------\n";
    cout << " Description:\n";
    cout << "   A battery-powered robot must travel from point A to\n";
    cout << "   point B across a square grid scattered with\n";
    cout << "   Battleship-style obstacles. It first moves horizontally\n";
    cout << "   to line up with B, then vertically, stepping around\n";
    cout << "   anything in its way. Every move costs 1 meter and 2%\n";
    cout << "   battery (50 moves max). If the battery runs out before\n";
    cout << "   it reaches B, the robot dies.\n";
    cout << "============================================================\n\n";
}
 
void printLegend() {
    cout << " Legend:\n";
    cout << "   " << SYM_ROBOT    << "  Robot (current position)\n";
    cout << "   " << SYM_START    << "  Point A (start)\n";
    cout << "   " << SYM_GOAL     << "  Point B (goal)\n";
    cout << "   " << SYM_OBSTACLE << "  Obstacle\n";
    cout << "   " << SYM_PATH     << "  Path already travelled\n";
    cout << "   " << SYM_EMPTY    << "  Open space\n\n";
    cout << " Coordinates are entered as (row, column), starting at 1\n";
    cout << " in the top-left corner.\n\n";
}
 
// ------------------------------ Input --------------------------------------
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
 
Point readPoint(const string& name) {
    cout << "\n Enter coordinates for point " << name << ":\n";
    Point p;
    p.r = readInt("   Row   ", 1, side) - 1;
    p.c = readInt("   Column", 1, side) - 1;
    return p;
}
 
// ------------------------- Obstacle generation -----------------------------
void clearGrid() {
    for (int r = 0; r < MAX_SIDE; r++)
        for (int c = 0; c < MAX_SIDE; c++)
            grid[r][c] = EMPTY;
}
 
// Places obstacles Battleship-style; returns how many were placed.
int placeObstacles(int count, int size, Point A, Point B, mt19937& rng) {
    clearGrid();
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
                if (grid[r][c] != EMPTY || samePoint(p, A) || samePoint(p, B))
                    ok = false;
            }
            if (ok) {
                for (int k = 0; k < size; k++) {
                    int r = r0 + (horizontal ? 0 : k);
                    int c = c0 + (horizontal ? k : 0);
                    grid[r][c] = OBSTACLE;
                }
                placed++;
                break;
            }
        }
    }
    return placed;
}
 
// Breadth-first search: confirms a route from A to B exists at all.
bool pathExists(Point A, Point B) {
    bool seen[MAX_SIDE][MAX_SIDE] = {};
    const int dr[4] = {-1, 1, 0, 0};
    const int dc[4] = {0, 0, -1, 1};
    queue<Point> q;
    q.push(A);
    seen[A.r][A.c] = true;
 
    while (!q.empty()) {
        Point p = q.front();
        q.pop();
        if (samePoint(p, B)) return true;
        for (int d = 0; d < 4; d++) {
            int nr = p.r + dr[d], nc = p.c + dc[d];
            if (isFree(nr, nc) && !seen[nr][nc]) {
                seen[nr][nc] = true;
                q.push(Point{nr, nc});
            }
        }
    }
    return false;
}
 
// ------------------------------ Display ------------------------------------
string cellSymbol(int r, int c, Point robot, Point A, Point B) {
    if (r == robot.r && c == robot.c) return SYM_ROBOT;
    if (r == A.r && c == A.c)         return SYM_START;
    if (r == B.r && c == B.c)         return SYM_GOAL;
    switch (grid[r][c]) {
        case OBSTACLE: return SYM_OBSTACLE;
        case VISITED:  return SYM_PATH;
        default:       return SYM_EMPTY;
    }
}
 
void render(Point robot, Point A, Point B, int moves, int distance,
            int battery, const string& status) {
    clearScreen();
    cout << "=================== ROBOT PATH NAVIGATOR ===================\n\n";
 
    cout << "     ";
    for (int c = 0; c < side; c++) cout << setw(2) << c + 1;
    cout << "\n    +" << string(side * 2 + 1, '-') << "+\n";
 
    for (int r = 0; r < side; r++) {
        cout << setw(3) << r + 1 << " |";
        for (int c = 0; c < side; c++)
            cout << " " << cellSymbol(r, c, robot, A, B);
        cout << " |\n";
    }
    cout << "    +" << string(side * 2 + 1, '-') << "+\n\n";
 
    int filled = battery / 5;   // 20-segment bar
    cout << " Moves    : " << moves << "\n";
    cout << " Distance : " << distance << " m\n";
    cout << " Battery  : [" << string(filled, '=') << string(20 - filled, ' ')
         << "] " << battery << "%\n";
    cout << " Position : (" << robot.r + 1 << ", " << robot.c + 1 << ")"
         << "    Goal B: (" << B.r + 1 << ", " << B.c + 1 << ")\n";
    cout << " Status   : " << status << "\n\n";
    cout << " " << SYM_ROBOT << " robot  " << SYM_START << " start  "
         << SYM_GOAL << " goal  " << SYM_OBSTACLE << " obstacle  "
         << SYM_PATH << " path  " << SYM_EMPTY << " open\n" << flush;
}
 
// -------------------------------- Main -------------------------------------
int main() {
    setupConsole();
    printHeader();
    printLegend();
 
    // ---- Setup inputs ----
    side = readInt(" Enter the grid side length", MIN_SIDE, MAX_SIDE);
    int obstacleSize = side / 2;               // integer division rounds down
    cout << "   Each obstacle will be " << obstacleSize << " cell(s) long.\n";
    int obstacleCount = readInt(" Enter the number of obstacles", 0, side - 1);
 
    Point A = readPoint("A (start)");
    Point B;
    while (true) {
        B = readPoint("B (goal)");
        if (!samePoint(A, B)) break;
        cout << "   Point B must be different from point A.\n";
    }
 
    // ---- Generate obstacle layout ----
    mt19937 rng(random_device{}());
    bool validLayout = false;
    int placed = 0;
    for (int attempt = 0; attempt < LAYOUT_ATTEMPTS && !validLayout; attempt++) {
        placed = placeObstacles(obstacleCount, obstacleSize, A, B, rng);
        validLayout = (placed == obstacleCount) && pathExists(A, B);
    }
    if (!validLayout) {
        cout << "\n   Warning: could not build a layout with all " << obstacleCount
             << " obstacles and a guaranteed route.\n"
             << "   Using a layout with " << placed
             << " obstacle(s); B may be unreachable.\n";
    }
 
    // ---- Initialize robot state ----
    int   moves    = 0;
    int   distance = 0;
    int   battery  = BATTERY_START;
    Point robot    = A;
 
    cout << "\n Press ENTER to begin the simulation...";
    string dummy;
    getline(cin, dummy);
 
    auto moveRobot = [&](int dr, int dc) {
        grid[robot.r][robot.c] = VISITED;
        robot.r += dr;
        robot.c += dc;
        moves++;
        distance += METERS_PER_MOVE;
        battery  -= BATTERY_PER_MOVE;
    };
 
    render(robot, A, B, moves, distance, battery, "Robot powered on at point A");
    pauseMs(STEP_DELAY_MS);
 
    // ---- Navigation state ----
    Phase   phase       = HORIZONTAL;
    int     lane        = robot.r;    // the original path (row or column) being followed
    bool    detouring   = false;
    bool    lastForward = false;      // last move was along the travel direction
    bool    retreating  = false;      // backing out of a dead end
    int     sideDir     = 1;          // +1 / -1 perpendicular side-step direction
    Outcome outcome     = RUNNING;
    uniform_int_distribution<int> coin(0, 1);
 
    // ---- Simulation loop ----
    while (outcome == RUNNING) {
        if (samePoint(robot, B)) { outcome = REACHED; break; }
 
        string note;
        if (phase == HORIZONTAL && robot.c == B.c) {
            phase = VERTICAL;  lane = robot.c;
            detouring = false; lastForward = false; retreating = false;
            note = "Lined up with B - switching to vertical. ";
        } else if (phase == VERTICAL && robot.r == B.r) {
            phase = HORIZONTAL; lane = robot.r;
            detouring = false;  lastForward = false; retreating = false;
            note = "Re-aligning horizontally. ";
        }
 
        // Battery check before every move
        if (battery < BATTERY_PER_MOVE) { outcome = BATTERY_DEAD; break; }
 
        // Forward direction (toward B on current axis) and perpendicular axis
        int fdr, fdc, pdr, pdc, perpOff, perpToGoal;
        if (phase == HORIZONTAL) {
            fdr = 0; fdc = (B.c > robot.c) ? 1 : -1;
            pdr = 1; pdc = 0;
            perpOff = robot.r - lane;  perpToGoal = B.r - robot.r;
        } else {
            fdr = (B.r > robot.r) ? 1 : -1; fdc = 0;
            pdr = 0; pdc = 1;
            perpOff = robot.c - lane;  perpToGoal = B.c - robot.c;
        }
 
        string status;
        bool moved = false;
 
        // Normal travel: check the path ahead first
        if (!detouring) {
            if (isFree(robot.r + fdr, robot.c + fdc)) {
                moveRobot(fdr, fdc);
                lastForward = true;
                moved = true;
                status = "Path clear - moving " + dirName(fdr, fdc);
            } else {
                detouring   = true;
                lastForward = false;
                if (perpToGoal > 0)      sideDir = 1;
                else if (perpToGoal < 0) sideDir = -1;
                else                     sideDir = coin(rng) ? 1 : -1;
                note += "Obstacle ahead! ";
            }
        }
 
        // Detour: work around the obstacle, then return to the original path
        if (!moved) {
            int backDir = (perpOff > 0) ? -1 : 1;   // direction back toward lane
            int sr = sideDir * pdr, sc = sideDir * pdc;
 
            if (perpOff != 0 && lastForward &&
                isFree(robot.r + backDir * pdr, robot.c + backDir * pdc)) {
                moveRobot(backDir * pdr, backDir * pdc);
                lastForward = false;
                if (abs(perpOff) == 1) {
                    detouring = false;
                    status = "Obstacle cleared - back on original path";
                } else {
                    status = "Stepping " + dirName(backDir * pdr, backDir * pdc)
                           + " toward original path";
                }
            } else if (!retreating && isFree(robot.r + fdr, robot.c + fdc)) {
                moveRobot(fdr, fdc);
                lastForward = true;
                if (perpOff == 0) {
                    detouring = false;
                    status = "Path clear - moving " + dirName(fdr, fdc);
                } else {
                    status = "Moving " + dirName(fdr, fdc) + " alongside obstacle";
                }
            } else if (isFree(robot.r + sr, robot.c + sc)) {
                moveRobot(sr, sc);
                lastForward = false;
                retreating  = false;
                status = "Blocked - side-stepping " + dirName(sr, sc);
            } else if (isFree(robot.r - sr, robot.c - sc)) {
                sideDir = -sideDir;
                moveRobot(-sr, -sc);
                lastForward = false;
                retreating  = false;
                status = "That side is blocked - side-stepping " + dirName(-sr, -sc);
            } else if (isFree(robot.r - fdr, robot.c - fdc)) {
                moveRobot(-fdr, -fdc);
                lastForward = false;
                retreating  = true;
                status = "Dead end - backing up " + dirName(-fdr, -fdc);
            } else {
                outcome = TRAPPED;
                status  = "No possible moves";
            }
        }
 
        render(robot, A, B, moves, distance, battery, note + status);
        if (outcome == RUNNING) pauseMs(STEP_DELAY_MS);
    }
 
    // ---- Results ----
    cout << "\n============================================================\n";
    switch (outcome) {
        case REACHED:
            cout << "  SUCCESS! The robot reached point B.\n";
            break;
        case BATTERY_DEAD:
            cout << "  ROBOT DIED - battery depleted.\n"
                 << "  Failed to navigate to point B.\n";
            break;
        case TRAPPED:
            cout << "  ROBOT STUCK - boxed in by obstacles.\n"
                 << "  Failed to navigate to point B.\n";
            break;
        default:
            break;
    }
    cout << "------------------------------------------------------------\n";
    cout << "  Total moves       : " << moves << "\n";
    cout << "  Distance traveled : " << distance << " m\n";
    cout << "  Battery remaining : " << battery << "%\n";
    cout << "============================================================\n";
 
    return 0;
}