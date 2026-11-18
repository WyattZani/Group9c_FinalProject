/******************************************************************************
 * File        : input.h
 * Owner       : Project Leader (readInt, T0) / Isaiah (readPoint, T2)
 * Description : Validated keyboard input.
 ******************************************************************************/
#ifndef INPUT_H
#define INPUT_H

#include <string>
#include "point.h"

int   readInt(const std::string& prompt, int lo, int hi);
Point readPoint(const std::string& name, int side);

#endif
