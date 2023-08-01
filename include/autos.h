#pragma once 
#include "vex.h"

extern void boxTest();
extern void odomBoxTest();
extern void odomReturnToStartTest();
extern void flywheelTest();

extern void leftWP();         // Slot 1
extern void rightWP();        // 2
extern void deviousRightWP(); // 3

extern void soloWP();         // 4
extern void mysteryAuto();    // 5
// extern void nineDiscSoloWP();
extern int progSkills();      // 6