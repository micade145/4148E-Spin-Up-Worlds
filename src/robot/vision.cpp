#include "robot_header/vision.h"
// using signature = vision::signature;

// Vision PID
PID visionPID(0.05, 0.01);
bool enableTracking = false;

// Constants
double MAX_GOAL_WIDTH;
double MIN_GOAL_WIDTH;
double MAX_GOAL_HEIGHT;
double MIN_GOAL_HEIGHT;

// goal tracking task
double centerPower = 0;
int alignToGoalTask() {
  double maxCenterPower = 10;
  double centerError = 0;
  bool buttonHeld;
  while (true) {
    // Alliance color toggle
    if(Controller.ButtonX.pressing()) {
      if(!buttonHeld) {
        isRed = !isRed; 
        buttonHeld = true;
      }
    } else {buttonHeld = false;}

    // Vision tracking 
    isRed ? Vision.takeSnapshot(Vision__RED_GOAL) : Vision.takeSnapshot(Vision__BLUE_GOAL);
    // if(isRed) {Vision.takeSnapshot(Vision__RED_GOAL);}
    // else {Vision.takeSnapshot(Vision__BLUE_GOAL);}
    if (Vision.largestObject.exists) {
      // center of vision is 158
      // less than 158 = offset right ; greater than 158 = offset left
      if(angler.value() == true) {
        centerError = 130 - Vision.largestObject.centerX;
      }
      else {centerError = 140 - Vision.largestObject.centerX;}
      
      centerPower = visionPID.calculateOutput(centerError);
      centerPower = constrainValue(centerPower, maxCenterPower, -maxCenterPower);

      // if(MIN_GOAL_HEIGHT <= Vision.largestObject.height <= MAX_GOAL_HEIGHT)

      if(enableTracking) {
        leftDrive.spin(forward, -centerPower, volt);
        rightDrive.spin(forward, centerPower, volt);
      }

      wait(10, msec);
    }

    // Debug info
    Brain.Screen.setCursor(12, 1);
    if(isRed) {
      Brain.Screen.clearLine(12);
      Brain.Screen.setPenColor(red);
      Brain.Screen.print("We are RED");
    }
    else {
      Brain.Screen.clearLine(12);
      Brain.Screen.setPenColor(blue);
      Brain.Screen.print("We are BLUE");
    }

    Brain.Screen.setPenColor(white);
    
    wait(20, msec);
  }
  return(0);
}

// track goal during driver control, no task
double visionError;
double visionTurnPower;
void trackGoal() {
  enableTracking = true;
  if(isRed) {Vision.takeSnapshot(Vision__RED_GOAL);}
  else Vision.takeSnapshot(Vision__BLUE_GOAL);

  if (Vision.largestObject.exists) {
      // center of vision is 158
      // less than 158 = offset left ; greater than 158 = offset right
      if(angler.value() == true) {
        visionError = 162 - Vision.largestObject.centerX;
      }
      else {visionError = 162 - Vision.largestObject.centerX;}
      
      visionTurnPower = -(visionPID.calculateOutput(visionError));

      visionTurnPower = constrainValue(visionTurnPower, visionError, -visionError);

      // leftDrive.spin(forward, -visionTurnPower, volt);
      // rightDrive.spin(forward, visionTurnPower, volt);
    
      // wait(10, msec);
    }
}