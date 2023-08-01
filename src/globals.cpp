#include "globals.h"

// Alliance color
bool isRed = false;
void toggleAllianceColor() {
  isRed = !isRed;
}

// int toggleAllianceTask() {
//   bool buttonHeld;
//   while(true) {
//     if(Controller.ButtonX.pressing()) {
//       if(!buttonHeld) {
//         isRed = !isRed;
//         buttonHeld = true;
//       }
//     } 
//     else {buttonHeld = false;}
//     }
// }

// Conversion rates
double PCT_TO_VOLT = 12.7 / 100;
double VOLT_TO_PCT = 100 / 12.7;

// Math constants
double RAD_TO_DEG = 180 / M_PI;
double DEG_TO_RAD = M_PI / 180;

// Math functions
double constrainValue(double input, double max, double min) {
  if(input > max) { input = max; }
  if(input < min) { input = min; }
  return(input);
}

double constrainAngle180(double input) {
  while(!(input >= 0 && input < 360)) {
    if( input < 0 ) { input += 360; }
    if(input >= 360) { input -= 360; }
  }
  return(input);
}
double constrainAngle90(double input) {
  while(!(input >= -90 && input < 90)) {
    if( input < -90 ) { input += 180; }
    if(input >= 90) { input -= 180; }
  }
  return(input);
}

int returnSign(double input) {
  return (std::fabs(input) / input);
}