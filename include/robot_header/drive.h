// #ifndef DRIVE_H
// #define DRIVE_H
#pragma once
#include "vex.h"

// Constants
extern double DRIVE_SENS;

// Helper functions
extern void setDrive(double leftVolt, double rightVolt);
extern void resetDrivePosition();

// Global turn variable
extern double driveTurnPower;
extern double driveForwardPower;

// Driver control functions 
extern void splitArcadeVolt();
extern void splitArcadePct();
extern void curveSplitArcade();

// Drive brake modes
extern void setDriveBrakeMode(brakeType brakeMode);



// #endif