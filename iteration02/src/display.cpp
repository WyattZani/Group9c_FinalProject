/******************************************************************************
 * File        : display.cpp
 * Owner       : Wyatt (T4, T7)
 * Status      : STUB - replace each TODO with working code.
 *               Port from Group9FinalDraft1.cpp: setupConsole(),
 *               clearScreen(), pauseMs(), printHeader(), printLegend(),
 *               cellSymbol(), render().
 ******************************************************************************/
#include "display.h"
#include <iostream>

using namespace std;

void setupConsole() {
    // TODO(Wyatt): port the Windows UTF-8 console setup from the draft.
}

void clearScreen() {
    // TODO(Wyatt): clear the console.
}

void pauseMs(int ms) {
    // TODO(Wyatt): sleep for ms milliseconds.
    (void)ms;
}

// --------------------------- T7: header / legend ---------------------------
void printHeader() {
    // TODO(Wyatt): port printHeader() from the draft.
    cout << "=================== ROBOT PATH NAVIGATOR ===================\n";
}

void printLegend() {
    // TODO(Wyatt): port printLegend() from the draft.
    cout << "   [printLegend] not implemented yet\n";
}

// ----------------------------- T4: display ---------------------------------
string cellSymbol(const Grid& grid, int r, int c, Point robot, Point A, Point B) {
    // TODO(Wyatt): port cellSymbol() from the draft.
    (void)grid; (void)r; (void)c; (void)robot; (void)A; (void)B;
    return SYM_EMPTY;
}

void render(const Grid& grid, Point robot, Point A, Point B, int moves,
            int distance, int battery, const string& status) {
    // TODO(Wyatt): port render() from the draft.
    cout << "   [render] not implemented yet\n";
    (void)grid; (void)robot; (void)A; (void)B;
    (void)moves; (void)distance; (void)battery; (void)status;
}
