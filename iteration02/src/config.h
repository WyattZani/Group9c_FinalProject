/******************************************************************************
 * File        : config.h
 * Owner       : Project Leader (T0)
 * Description : Shared constants and display symbols used by every file.
 ******************************************************************************/
#ifndef CONFIG_H
#define CONFIG_H

#include <string>

const bool USE_UNICODE = true;   // set false if your console shows garbled symbols

const int MIN_SIDE         = 3;
const int MAX_SIDE         = 10;
const int BATTERY_START    = 100;
const int BATTERY_PER_MOVE = 2;
const int METERS_PER_MOVE  = 1;
const int STEP_DELAY_MS    = 500;
const int LAYOUT_ATTEMPTS  = 500;

// Display symbols
const std::string SYM_EMPTY    = USE_UNICODE ? "⁻" : ".";   // open space
const std::string SYM_OBSTACLE = USE_UNICODE ? "▧" : "#";   // obstacle
const std::string SYM_PATH     = USE_UNICODE ? "∺" : "*";   // path travelled
const std::string SYM_ROBOT    = "R";
const std::string SYM_START    = "A";
const std::string SYM_GOAL     = "B";

#endif
