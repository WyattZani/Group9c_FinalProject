/******************************************************************************
 * File        : display.cpp
 * Owner       : Wyatt (T4, T7)
 * Status      : DONE - ported from Group9FinalDraft1.cpp: setupConsole(),
 *               clearScreen(), pauseMs(), printHeader(), printLegend(),
 *               cellSymbol(), render().
 ******************************************************************************/
#define NOMINMAX
#include "display.h"
#include <iostream>
#include <iomanip>
#include <thread>
#include <chrono>
#ifdef _WIN32
  #include <windows.h>
  #ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
    #define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
  #endif
#endif

using namespace std;

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

// --------------------------- T7: header / legend ---------------------------
void printHeader() {
    cout << "============================================================\n";
    cout << "                   ROBOT PATH NAVIGATOR\n";
    cout << "============================================================\n";
    cout << " Program  : Robot Path Navigator (obstacle avoidance sim)\n";
    cout << " Authors  : Group 9C - Wyatt Zani, Robert, Isaiah\n";
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

// ----------------------------- T4: display ---------------------------------
string cellSymbol(const Grid& grid, int r, int c, Point robot, Point A, Point B) {
    if (r == robot.r && c == robot.c) return SYM_ROBOT;
    if (r == A.r && c == A.c)         return SYM_START;
    if (r == B.r && c == B.c)         return SYM_GOAL;
    switch (grid.getCell(r, c)) {
        case OBSTACLE: return SYM_OBSTACLE;
        case VISITED:  return SYM_PATH;
        default:       return SYM_EMPTY;
    }
}

void render(const Grid& grid, Point robot, Point A, Point B, int moves,
            int distance, int battery, const string& status) {
    int side = grid.getSize();
    clearScreen();
    cout << "=================== ROBOT PATH NAVIGATOR ===================\n\n";

    cout << "     ";
    for (int c = 0; c < side; c++) cout << setw(2) << c + 1;
    cout << "\n    +" << string(side * 2 + 1, '-') << "+\n";

    for (int r = 0; r < side; r++) {
        cout << setw(3) << r + 1 << " |";
        for (int c = 0; c < side; c++)
            cout << " " << cellSymbol(grid, r, c, robot, A, B);
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
