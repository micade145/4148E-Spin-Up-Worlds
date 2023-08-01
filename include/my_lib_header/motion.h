#pragma once 
#include "vex.h"

// Constants 
extern double DRIVE_INCH_TO_DEG;
extern double DRIVE_DEG_TO_INCH;

extern void Move(double driveTarget, double maxDrivePower, double turnTarget, double maxTurnPower, 
          int maxTime, bool driveSlew, bool turnSlew);
extern void Turn(double targetAngle, double maxTurnPower, int maxTime);
// extern void MoveToPoint(double targetX, double targetY, double endOrientation, double maxTranslatePower, 
//                         double maxRotatePower, double maxOrientPower, int maxTime);
extern void MoveToPoint(double targetX, double targetY, double endOrientation, double maxTranslatePower, 
                        double maxRotatePower, double maxOrientPower, int maxTime);

