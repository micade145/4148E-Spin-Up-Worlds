#pragma once
#include "vex.h"

// Driver control function
extern void intakeControl();

// Shooting functions
void shootDisc(int msecDelay);
void shoot(int discNum, int msecDelay);

void singleShot();
void tripleShot();

// Intake tasks
extern int intakeToggle();
extern int autoEject();

