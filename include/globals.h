// #ifndef GLOBALS_H
// #define GLOBALS_H
#pragma once
#include "vex.h"

// Alliance color
extern bool isRed;
extern void toggleAllianceColor();

// Conversion rates
extern double PCT_TO_VOLT;
extern double VOLT_TO_PCT;

// Math constants
extern double RAD_TO_DEG;
extern double DEG_TO_RAD;

// Math functions
extern double constrainValue(double input, double max, double min);
extern double constrainAngle180(double input);
extern double constrainAngle90(double input);
extern int returnSign(double input);
// #endif