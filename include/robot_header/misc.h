#pragma once 
#include "vex.h"

// Variables
extern bool indexState;

// Pneumatic functions
extern void toggleAngler();
// extern void indexDisc();
// extern void shoot(int discNum, int msecDelay);

// Endgame toggles
extern void toggleLeftEndgame();
extern void toggleRightEndgame();
extern void toggleBottomEndgame();
extern void toggleAllEndgame();

