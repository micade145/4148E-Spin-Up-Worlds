#pragma once
#include "vex.h" 

// Constants
extern double FW_VOLT_HIGH;
extern double FW_VOLT_LOW;
extern double FW_RPM_TO_VOLT;
extern double FW_VOLT_TO_RPM;
extern double TBH_kI;

// Variables
extern double flywheelTarget;
extern double flywheelVoltage;

// Drive Control Functions
extern void toggleFlywheel(); // doesn't work, registers multiple toggles
extern int flywheelControl(); // drive control toggle
extern void spinUpFlywheel(double newFlywheelTarget); // spin up task

// Control Loops
extern int flywheelBangBang();
extern void flywheelPID();
extern void flywheelTBH();

