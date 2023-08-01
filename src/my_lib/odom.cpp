#include "my_lib_header/odom.h"

// Constants
double SIDE_ENC_OFFSET = 4.8;
double LEFT_ENC_OFFSET = 5.5;

// Global points
Point globalPose;
Point localPose;
Point deltaPose;

// Odom variables
double currentEncValue, lastEncValue, deltaEncValue, lastHeading;
double sideEncValue, lastSideEncValue, deltaSideEncValue;
double polarAngle, globalPolarAngle, polarLength;

// Odom functions
void resetOdomSensors() {
  frontEnc.resetPosition();
  sideEnc.resetPosition();
  Inertial.resetRotation();
}

void updateSensors() {
  // Update front encoder in inches
  currentEncValue = frontEnc.position(deg) * (2.75 * M_PI) / 360;
  deltaEncValue = currentEncValue - lastEncValue;
  
  // update side encoder in inches
  sideEncValue = sideEnc.position(deg) * (2.75 * M_PI) / 360;
  deltaSideEncValue = sideEncValue - lastSideEncValue;

  // update global theta in radians
  globalPose.theta = Inertial.rotation() * DEG_TO_RAD;
  deltaPose.theta = globalPose.theta - lastHeading;

  // update previous values
  lastEncValue = currentEncValue;
  lastSideEncValue = sideEncValue;
  lastHeading = globalPose.theta;
}

// void updatePosition() { // Doesn't work very well:(
//   deltaPose.x = inchesTraveled * sin(globalPose.theta);
//   deltaPose.y = inchesTraveled * cos(globalPose.theta);

//   globalPose.x += deltaPose.x;
//   globalPose.y += deltaPose.y;
// } 


void updatePosition() {
  if(deltaPose.theta == 0) {
    localPose.x = deltaSideEncValue;
    localPose.y = deltaEncValue;
  }
  else {
    localPose.x = (2 * sin(deltaPose.theta / 2)) * ((deltaSideEncValue / deltaPose.theta) + SIDE_ENC_OFFSET);
    localPose.y = (2 * sin(deltaPose.theta / 2)) * ((deltaEncValue / deltaPose.theta));
  }

  if(localPose.x == 0 && localPose.y == 0) {
    polarAngle = 0;
    polarLength = 0;
  }
  else {
    polarAngle = atan2(localPose.y, localPose.x);
    polarLength = sqrt(pow(localPose.x, 2) + pow(localPose.y, 2));
  }
  
  globalPolarAngle = polarAngle - lastHeading  - (deltaPose.theta / 2);

  deltaPose.x = polarLength * cos(globalPolarAngle);
  deltaPose.y = polarLength * sin(globalPolarAngle);

  globalPose.x += deltaPose.x;
  globalPose.y += deltaPose.y;
}

// void updatePosition() {
//   // backOffset = radius, deltaBackEnc = arc length;
//   // deltaBackEnc/backOffset = angle, then add deltaAngle
//   // multiply by backOffset (radius) to give arc length

//   localPose.x = ((deltaSideEncValue/SIDE_ENC_OFFSET) - deltaPose.theta) * SIDE_ENC_OFFSET; 
//   localPose.y = deltaEncValue;

//   deltaPose.x = (localPose.y * sin(lastHeading + deltaPose.theta/2)) + (localPose.x * cos(lastHeading + deltaPose.theta/2));
//   deltaPose.y = (localPose.y * cos(lastHeading + deltaPose.theta/2)) + (localPose.x * sin(lastHeading + deltaPose.theta/2));

//   globalPose.x += deltaPose.x;
//   globalPose.y += deltaPose.y;
// }

// Position tracking task
int trackPosition() {
  // resetDrivePosition();
  // resetOdomSensors();
  // globalPose.reset();

  while(true) {
    // Update
    updateSensors();
    updatePosition();

    // Print info
    Brain.Screen.clearLine(12);
    Brain.Screen.setCursor(12,1);
    Brain.Screen.print("X: %3.2f   Y: %3.2f   Angle: %3.2f", 
    globalPose.x, globalPose.y, (globalPose.theta * RAD_TO_DEG)); 

    // longer code but keeps text from shifting when updating
    // Brain.Screen.print("X: %3.2f", globalPose.x);     
    // Brain.Screen.setCursor(11, 13);
    // Brain.Screen.print("Y: %3.2f", globalPose.y);
    // Brain.Screen.setCursor(11,25);
    // Brain.Screen.print("Angle: %3.2f", globalPose.theta);    
    
    wait(15, msec);
  }
  return(0);
}
