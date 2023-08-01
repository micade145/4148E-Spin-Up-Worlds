#include "robot_header/drive.h" 

// Constants
double DRIVE_SENS = 0.8;

// Drive Helper Function
void setDrive(double leftVolt, double rightVolt) {
    leftDrive.spin(forward, leftVolt, volt);
    rightDrive.spin(forward, rightVolt, volt);
}
void resetDrivePosition() {
  leftDrive.resetPosition();
  rightDrive.resetPosition();
}

// Driver Control functions
double driveTurnPower;
double driveForwardPower;
void splitArcadeVolt() {
    driveForwardPower = Controller.Axis3.position(percent) * (12.7 / 100);
    driveTurnPower = Controller.Axis1.position(percent) * (12.7 / 100) * DRIVE_SENS;
    // Deadzone 
    if(fabs(driveForwardPower) <= 1) {driveForwardPower = 0;}
    if(fabs(driveTurnPower) <= 1) {driveTurnPower = 0;}
    setDrive(driveForwardPower + driveTurnPower, driveForwardPower - driveTurnPower);
}

void splitArcadePct() {
  double forwardPower = Controller.Axis3.position(percent) ;
  double turnPower = Controller.Axis1.position(percent) * DRIVE_SENS;
  // Deadzone
  if(fabs(forwardPower) <= 10) { forwardPower = 0;}
  if(fabs(turnPower) <= 10) { turnPower = 0;}
  leftDrive.spin(forward, forwardPower + turnPower, percent);
  rightDrive.spin(forward, forwardPower - turnPower, percent);
}

void curveSplitArcade() {
  // Read controller input
  double forwardPower = Controller.Axis3.position(percent) * PCT_TO_VOLT;
  double turnPower = Controller.Axis1.position(percent) * PCT_TO_VOLT * DRIVE_SENS;
  
  // Exponential (x^3) curve on turning
  turnPower = pow(turnPower, 3) / pow(12.7, 2);
  
  // Deadzone
  if(fabs(forwardPower) <= 1) { forwardPower = 0;}
  if(fabs(turnPower) <= 1) { turnPower = 0;}

  // Output
  setDrive(forwardPower + turnPower, forwardPower - turnPower);
}

// Quick functions to set drive brake type
void setDriveBrakeMode(brakeType brakeMode) {
    leftDrive.setStopping(brakeMode);
    rightDrive.setStopping(brakeMode);
}

