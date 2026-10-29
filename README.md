# Robot Path Navigator

**Group 9C - Computing Fundamentals Final Project**
Wyatt Zani · Robert · Isaiah

A C++ console program that simulates a battery-powered robot navigating a square grid from
point A to point B. The grid is filled with randomly placed, Battleship-style obstacles. The
robot first moves horizontally to line up with B, then vertically, stepping around anything in
its way and returning to its original path once clear. Every move costs 1 meter and 2% battery
(50 moves max). If the battery runs out before the robot reaches B, the robot dies.

---

## Build

Requires a C++17 compiler From the repository root:

```bash
g++ -std=c++17 -Wall src/*.cpp -o robot_navigator
```

## Run

```bash
./robot_navigator          # macOS / Linux
robot_navigator.exe        # Windows
```

If the grid symbols look garbled in your console, set `USE_UNICODE = false` in `src/config.h`
and rebuild. The program will then use plain ASCII symbols.

---

## Menu

Each menu option runs exactly one task.

| Option | Task | What it does |
|:---:|---|---|
| 1 | T1 - Configure grid | Set the grid side length (3-10) and number of obstacles |
| 2 | T2 - Set start and goal | Enter coordinates for point A (start) and point B (goal) |
| 3 | T3 - Generate obstacles | Randomly place obstacles, guaranteeing a route from A to B exists |
| 4 | T4 - Show grid | Display the current grid with A, B, and obstacles |
| 5 | T5 - Run simulation | Animate the robot travelling from A to B |
| 6 | T6 - Show results | Outcome, moves, distance, and battery left from the last run |
| 7 | T7 - Help and legend | Program description, map symbols, and coordinate rules |
| 8 | T8 - Reset / new map | Clear the grid, points, and last run |
| 9 | T9 - Save / load map | Save the current map to a file, or load one *(optional)* |
| 10 | T10 - Run history | Success rate and average moves across all runs *(optional)* |
| 0 | Exit | Quit the program |

**Typical run:** 1 → 2 → 3 → 4 → 5 → 6.

### Map legend

| Symbol | Meaning |
|:---:|---|
| `R` | Robot (current position) |
| `A` | Point A (start) |
| `B` | Point B (goal) |
| `▧` / `#` | Obstacle |
| `∺` / `*` | Path already travelled |
| `⁻` / `.` | Open space |

Coordinates are entered as (row, column), starting at 1 in the top-left corner.

---

## Project structure

```
Group9c_FinalProject/
├── src/
│   ├── main.cpp          Menu loop (switch statement, one case per task)
│   ├── tasks.h / .cpp    One function per menu option + shared AppState
│   ├── config.h          Constants (grid limits, battery, symbols)
│   ├── point.h           Point struct (row, column)
│   ├── grid.h / .cpp     Grid class - cells, obstacles, route check, save/load
│   ├── robot.h / .cpp    Robot class - position, moves, distance, battery
│   ├── navigator.h/.cpp  Navigator class - drives the robot from A to B
│   ├── display.h / .cpp  Console drawing - grid, header, legend
│   └── input.h / .cpp    Validated keyboard input
├── docs/
│   ├── Project_Plan.xlsx        Task table, timeline, contributions, meetings
│   └── Iteration02_Report.pdf   System design and progress report
└── README.md
```

### Classes

| Class | Private data | Public methods |
|---|---|---|
| `Grid` | `side`, `cells[MAX_SIDE][MAX_SIDE]` | `setSize`, `getSize`, `clear`, `inBounds`, `isFree`, `getCell`, `markVisited`, `clearVisited`, `placeObstacles`, `pathExists`, `save`, `load` |
| `Robot` | `pos`, `moves`, `distance`, `battery` | `reset`, `move`, `hasBattery`, `getPosition`, `getMoves`, `getDistance`, `getBattery` |
| `Navigator` | `grid`, `robot`, `A`, `B`, `rng`, `phase`, `lane`, `detouring`, `lastForward`, `retreating`, `sideDir`, `outcome` | `run`, `step`, `getOutcome`, `getResult` |

### How the pieces connect

```
main.cpp ──► tasks.cpp ──┬──► Grid
                         ├──► Navigator ──► Robot
                         │        └──────► Grid (reference)
                         ├──► display  ──► Grid
                         └──► input
```

`main.cpp` shows the menu and calls one task function per option. Tasks share data through an
`AppState` struct (the grid, points A and B, last run, and run history). `Navigator` owns a
`Robot` and holds a reference to the `Grid` it travels on.

---

## Team and task ownership

| Member | Tasks | Files |
|---|---|---|
| Project Leader (Iteration 2): ROBERT | T0 menu and skeleton, T11 exit, D3 Excel plan | `main.cpp`, `tasks.h`, `config.h`, `point.h`, `input.cpp` (`readInt`) |
| Isaiah | T1 configure grid, T2 set points, T3 obstacles, T9 save/load | `grid.h/.cpp`, `input.cpp` (`readPoint`), Isaiah section of `tasks.cpp` |
| Robert | T5 run simulation, T6 results, T10 run history | `robot.h/.cpp`, `navigator.h/.cpp`, Robert section of `tasks.cpp` |
| Wyatt Zani | T4 show grid, T7 help and legend, T8 reset, D1 README | `display.h/.cpp`, Wyatt section of `tasks.cpp`, `README.md` |
| Everyone | D2 Iteration 2 report | `docs/Iteration02_Report.pdf` |

## Status

Status values: Not started / Stub / In progress / Done. The full task table is in
[`docs/Project_Plan.xlsx`](docs/Project_Plan.xlsx).

| Task | Status |
|---|---|
| T0 Menu and skeleton | Stub |
| T1 Configure grid | Stub |
| T2 Set start and goal | Stub |
| T3 Generate obstacles | Stub |
| T4 Show grid | FINISHED |
| T5 Run simulation | Stub |
| T6 Show results | Stub |
| T7 Help and legend | Stub |
| T8 Reset / new map | Stub |
| T9 Save / load map | Stub |
| T10 Run history | Stub |

## Tools

C++17 · VS Code · g++ / MSVC · Git and GitHub · GitHub Issues for task tracking ·
group chat for communication.
