#include "robot_header/flywheel.h"

// Constants
double FW_VOLT_HIGH = 13.0;
double FW_VOLT_LOW = 9.8;
double FW_RPM_TO_VOLT = 12.7 / 600;
double FW_VOLT_TO_RPM = 600 / 12.7;

// Variables
double flywheelTarget = 0.0;
double flywheelVoltage = 0.0;

// Drive Control functions
void toggleFlywheel() { // doesn't work -likely refreshes too fast in opcontrol and registers multiple toggles
  if(flywheelTarget != FW_VOLT_LOW * FW_VOLT_TO_RPM) {flywheelTarget = FW_VOLT_LOW * FW_VOLT_TO_RPM;}
  else {flywheelTarget = 0;}
}

bool buttonHeld = false;
int flywheelControl() {  // good one that works
  while(true) {
	if(Controller.ButtonLeft.pressing()) {
		if(!buttonHeld) {
			if(flywheelTarget != FW_VOLT_LOW) {flywheelTarget = FW_VOLT_LOW;}
			else {flywheelTarget = 0;}
			buttonHeld = true;
		}
	} 
	else {buttonHeld = false;}

  // If not using a control loop
  flywheel.spin(forward, flywheelTarget, volt);

  // Debug
  Brain.Screen.setCursor(11, 1);
  Brain.Screen.print("FW Target: %2.1f  | ", flywheelTarget);
  Brain.Screen.setCursor(11, 21);
  Brain.Screen.print("RPM: %3.1f", flywheel.velocity(rpm));

  wait(20, msec);
  }
  return(0);
}

void spinUpFlywheel(double newFlywheelTarget) {
  flywheelTarget = 13;
  wait(3, sec);
  flywheelTarget = newFlywheelTarget;
}

// Control Loops
// int flywheelBangBang() { // at least 700 msec to regain speed after a shot @ 10.6 V
//   double target = 0.0;
//   double fwCurrentVelocity = 0.0;
//   double fwError = 0.0;
//   double fwOutput = 0.0;
//   double fwSettleTime = 0.0;
//   double tempFwSettleTime = 0.0;
//   double avgFwError = 0.0;
//   double prevFwError;
//   int holdCount = 0;
//   while(true) {
//   target = flywheelTarget * (600 / 12.7);
//   fwCurrentVelocity = flywheel.velocity(rpm); 

//   fwError = target - fwCurrentVelocity;
//   avgFwError = (fwError + prevFwError) / 2;

//   if(avgFwError >= 70) { // used to check if indexer.value() == true 
//     fwOutput = 13;
//     fwSettleTime += 10;
//     // tempFwSettleTime += 10;
//     // holdCount = 0; 
//   }
//   if(avgFwError < 70){
//     fwOutput = flywheelTarget;
//     // if(holdCount < 1) {
//     //   fwSettleTime = tempFwSettleTime;
//     //   tempFwSettleTime = 0;
//     // }
//     // holdCount ++;
//   }

//   flywheel.spin(forward, fwOutput, volt);

//   prevFwError = fwError;

//   // Debug
//   Brain.Screen.clearLine(11);
//   Brain.Screen.setCursor(11, 1);
//   Brain.Screen.print("Set. Time: %3.1f |", fwSettleTime);
//   Brain.Screen.setCursor(11, 15);
//   Brain.Screen.print("Err: %3.1f |", avgFwError);

//   Brain.Screen.setCursor(11, 27);
//   Brain.Screen.print("Out: %2.1f |", fwOutput);
//   Brain.Screen.setCursor(11, 38);
//   Brain.Screen.print("Velo: %3.1f", fwCurrentVelocity);

//   wait(10, msec);
//   }
//   return(0);
// }

// int flywheelBangBang() { // better bang bang?
//   double target = 0.0;
//   double fwCurrentVelocity = 0.0;
//   double fwError = 0.0;
//   double fwOutput = 0.0;
//   double fwSettleTime = 0.0;
//   double tempFwSettleTime = 0.0;
//   double avgFwError = 0.0;
//   double prevFwError;
//   bool bangBangMode = true;
//   while(true) {
//   target = flywheelTarget * (600 / 12.7);
//   fwCurrentVelocity = flywheel.velocity(rpm); 

//   fwError = target - fwCurrentVelocity;
//   avgFwError = (fwError + prevFwError) / 2;

//   if(avgFwError >= 70) {
//     bangBangMode = true;
//   }
//   if(bangBangMode) {
//     fwOutput = 13;
//     fwSettleTime += 10;
//   }
//   if(avgFwError < 70){
//     bangBangMode = false;
//     fwOutput = flywheelTarget;
//   }

//   flywheel.spin(forward, fwOutput, volt);

//   prevFwError = fwError;

//   // Debug
//   Brain.Screen.clearLine(11);
//   Brain.Screen.setCursor(11, 1);
//   Brain.Screen.print("Set. Time: %3.1f |", fwSettleTime);
//   Brain.Screen.setCursor(11, 15);
//   Brain.Screen.print("Err: %3.1f |", avgFwError);

//   Brain.Screen.setCursor(11, 27);
//   Brain.Screen.print("Out: %2.1f |", fwOutput);
//   Brain.Screen.setCursor(11, 38);
//   Brain.Screen.print("Velo: %3.1f", fwCurrentVelocity);

//   wait(10, msec);
//   }
//   return(0);
// }

// int flywheelBangBang() { // bang bang + PID
//   double target = 0.0;
//   double fwCurrentVelocity = 0.0;
//   double fwError = 0.0;
//   double fwOutput = 0.0;
//   double fwSettleTime = 0.0;
//   double avgFwError = 0.0;
//   double prevFwError = 0.0;
//   int holdCount = 0;
//   bool spinUpMode = false;
//   bool pidMode = false;
//   bool holdMode = false;
//   while(true){
//   target = flywheelTarget * (600 / 12.7);
//   fwCurrentVelocity = flywheel.velocity(rpm); 

//   fwError = target - fwCurrentVelocity;
//   avgFwError = (fwError + prevFwError) / 2;

//   if(avgFwError >= 70) { // used to check if indexer.value() == true 
//     spinUpMode = true;
//     pidMode = holdMode = false;
//   }
//   if(spinUpMode) {
//     fwOutput = 13;
//     fwSettleTime += 10;
//   }
//   if(avgFwError < 80){
//     spinUpMode = holdMode = false;
//     pidMode = true;
//   }
//   if(pidMode) {
//     fwOutput = avgFwError * .5;
//   }
//   if(avgFwError < 50) {
//     holdCount += 10;
//   } else holdCount = 0;
  
//   if(holdCount >= 50) {
//     pidMode = spinUpMode = false;
//     holdMode = true;
//   }
//   if(holdMode) {
//     fwOutput = flywheelTarget;
//   }

//   fwOutput = constrainValue(fwOutput, 13, 0);
//   flywheel.spin(forward, fwOutput, volt);

//   prevFwError = fwError;

//   // Debug
//   Brain.Screen.clearLine(11);
//   Brain.Screen.setCursor(11, 1);
//   Brain.Screen.print("Set. Time: %3.1f |", fwSettleTime);
//   Brain.Screen.setCursor(11, 15);
//   Brain.Screen.print("Err: %3.1f |", avgFwError);

//   Brain.Screen.setCursor(11, 27);
//   Brain.Screen.print("Out: %2.1f |", fwOutput);
//   Brain.Screen.setCursor(11, 38);
//   Brain.Screen.print("Velo: %3.1f", fwCurrentVelocity);

//   wait(10, msec);
//   }
//   return(0);
// }

PID fwPID(1, 0);
void flywheelPID() {
  // Initialize variables
  double flywheelVelocity, flywheelError, flywheelPower;
    // while(true) {
        flywheelVelocity = flywheel.velocity(rpm);
        flywheelError = flywheelTarget - flywheelVelocity;

        flywheelPower = fwPID.calculateOutput(flywheelError);

        flywheelPower =  constrainValue(flywheelPower, 12.7, 0);

        flywheel.spin(forward, flywheelPower, volt);
        wait(20, msec);
    // }
    // return(0);
}

double TBH_kI = 1;
void flywheelTBH() {
  double flywheelError = 0.0;
  double prevFlywheelError = 0.0;
  double flywheelOutput = 0.0;
  double prevFlywheelOutput = 0.0;
  // while(true) {
    // Get error and calculate power
    flywheelError = flywheelTarget - flywheel.velocity(rpm);
    flywheelOutput += (flywheelError * TBH_kI);

    // Check if target is passed and take back half 
    if(flywheelError != 0 && returnSign(flywheelError) != returnSign(prevFlywheelError)) {
      flywheelOutput = (flywheelOutput + prevFlywheelOutput) / 2.0;
    }

    // Constrain power
    flywheelOutput = constrainValue(flywheelOutput, 12.7, 0);
    // Output to motors
    flywheel.spin(forward, flywheelOutput, volt);

    // Assigning prev variable values
    prevFlywheelError = flywheelError;
    prevFlywheelOutput = flywheelOutput;

    // wait(20, msec);
  // }
  // return(0);
}

void boolIndexing() {
      // flywheel.spin(forward, flywheelTarget + indexState*(13 - flywheelTarget), volt);
}