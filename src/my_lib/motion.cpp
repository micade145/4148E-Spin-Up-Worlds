#include "my_lib_header/motion.h"

// Constants
double DRIVE_INCH_TO_DEG = 360 / ((3.25*M_PI) * 4 / 6); //  ~ 360 / 6.8
double DRIVE_DEG_TO_INCH = ((3.25*M_PI) * 4 / 6) /360;  //  ~ 6.8 / 360

// PID objects
PID drivePID(.6, 0.6);
PID turnPID(.165, 0.55);

// old - constants tuned to degree values, no power when error is small
// PID drivePID(.00966, 0.00004);
// PID turnPID(.08, 0.000005); 

void Move(double target, double maxPower, double turnTarget, 
double maxTurn, int maxTime, bool driveSlew, bool turnSlew) {
  bool settled = false;
  // Reset Encoders
    resetDrivePosition();
    frontEnc.resetPosition();
  // Initialize timer
    timer DriveTimer;
    DriveTimer.reset();
    DriveTimer.time(msec);
  // Convert target inches to degrees
    // target *= 360 / (2.75 * M_PI);
  // Slew rate stuff - caps acceleration
    double slewRate = 0;
    double tempMax = 0;
    double tSlewRate = 0;
    double tTempMax = 0;
    double slewStart = 5;
    bool slew = false;
    bool tSlew = false;
    if (std::fabs(target) >= 18 && driveSlew) {
      slew = true;
      tempMax = slewStart;
      slewRate = maxPower / 100; // Lower = faster accel // 200 is good but slow
    }
    if (std::fabs((Inertial.rotation() - turnTarget)) > 90 && turnSlew) {
      tSlew = true;
      tTempMax = slewStart;
      tSlewRate = maxTurn / 60;
    }
  while (true){
    // double drivePosition = (leftDrive.position(degrees) + rightDrive.position(degrees)) / 2.0;
    double drivePosition = frontEnc.position(deg) * (2.75 * M_PI) / 360; /// multiply by (2.75 * M_PI) / 360;
    double driveHeading = Inertial.rotation();

    double driveError = target - drivePosition;
    double turnError = turnTarget - driveHeading;
    
    // Make sure it takes the shortest turn 
    if (turnError > 180) {
      turnError -=360;
    }
    if (turnError < -180) {
      turnError += 360;
    }

    // Calculate drive and turn power
    double drivePower = drivePID.calculateOutput(driveError);
    double turnPower = turnPID.calculateOutput(turnError);

    // Drive slew
    if (slew) {
      // Increment slew rate amount to max power
      tempMax += slewRate;
      // Cap max power
      tempMax = constrainValue(tempMax, maxPower, -maxPower);
      // Constrain drive power to max slew power
      drivePower = constrainValue(drivePower, tempMax, -tempMax);
    }
    // Turn Slew 
    if (tSlew) {
      // Increment turn slew rate amount to max power
      tTempMax += tSlewRate;
      // Cap max power
      tTempMax = constrainValue(tTempMax, maxTurn, -maxTurn);
      // Constrain drive power to max slew power
      turnPower = constrainValue(turnPower, tTempMax, -tTempMax);
    }   

    // Constrain power if not using slew rate
    //// Turn ////
    turnPower = constrainValue(turnPower, maxTurn, -maxTurn);
    // Drive //// 
    drivePower = constrainValue(drivePower, maxPower, -maxPower);

    // // Good? Exit Conditions
    if(std::fabs(driveError) <= 1 && std::fabs(turnError) <= 1.5){
      settled = true;
    }
    if(std::fabs(turnError) <= 1.5 && target == 0){
      settled = true;
    }
    if (DriveTimer >= maxTime){
      settled = true;
    }

    if(settled) {
      leftDrive.stop(brake);
      rightDrive.stop(brake);
      drivePower = 0;
      turnPower = 0;
      break;
    }

    // Print Debug Info
    Brain.Screen.clearScreen(black);
    Brain.Screen.setCursor(10, 1);
    Brain.Screen.print( "Drive Error: %4.1f", driveError);
    Brain.Screen.setCursor(10, 22);
    Brain.Screen.print("Power/Max: %3.1f / %3.1f", drivePower, maxPower);
    Brain.Screen.setCursor(11, 1);
    Brain.Screen.print( "Turn Error: %4.1f", turnError);
    Brain.Screen.setCursor(11, 22);
    Brain.Screen.print( "Power/Max: %3.1f / %3.1f", turnPower, maxTurn);
    Brain.Screen.setCursor(12, 1);
    Brain.Screen.print( "Slew/Max: %1.2f/%2.1f", slewRate, tempMax);
    Brain.Screen.setCursor(12, 22);
    Brain.Screen.print( "Turn/Max: %1.2f/%2.1f", tSlewRate, tTempMax);

    // Output To Motors
    leftDrive.spin(forward, drivePower + turnPower, volt);
    rightDrive.spin(forward, drivePower - turnPower, volt);
    
    wait(15, msec);
  }
  drivePID.reset();
  turnPID.reset();
}

timer turnTimer;
void Turn(double targetAngle, double maxTurnPower, int maxTime) {
  // restart timer
  turnTimer.reset();
  turnTimer.time(msec);
  // Initialize variables
  double currentAngle, turnError, turnPower;
  while(true) {
    // Errors
    currentAngle = Inertial.rotation();
    turnError = constrainAngle180(targetAngle - currentAngle);

    // Calculate and constrain power
    turnPower = turnPID.calculateOutput(turnError);
    turnPower = constrainValue(turnPower, maxTurnPower, -maxTurnPower);
    
    // Exit condition
    if(std::fabs(turnError) <= 5 || turnTimer >= maxTime) {
      leftDrive.stop(brake);
      rightDrive.stop(brake);
      break;
    }

    // Output to drive
    setDrive(turnPower, -turnPower);

    // Print info on brain
    Brain.Screen.clearScreen();
    Brain.Screen.setCursor(11, 1);
    Brain.Screen.print( "Turn Error: %4.1f", turnError);

    wait(20, msec);
  } 
  turnPID.reset();
  turnTimer.reset();
}

PID translatePID(.7, 3);
timer toPointTimer;

void MoveToPoint(double targetX, double targetY, double endOrientation, 
                double maxTranslatePower, double maxRotatePower, double maxOrientPower, int maxTime) {
  toPointTimer.reset();
  toPointTimer.time(msec);
  double distanceToTarget, targetAngle, angleError, orientationError, translatePower, rotatePower, xError, yError, moveScaleFactor;
  double orientPower = 0;
  int settledCount = 0;
  bool onTarget = false;
  bool moveFinished = false;
  while(true) {
    xError = targetX - globalPose.x;
    yError = targetY - globalPose.y;

    // Get errors
      distanceToTarget = hypot(xError, yError);
      targetAngle = fmod(((M_PI_2 - atan2(yError, xError)) * RAD_TO_DEG), 360);
      angleError = targetAngle - (globalPose.theta * RAD_TO_DEG);

    // Prioritize turning 
    // Larger angle = less drive power due to cos values; as angle error approaches 0, scale factor approaches 1
      moveScaleFactor = cos(fmod(angleError, 90) * DEG_TO_RAD); 

    if(onTarget == false) { // We still need to approach the point
      // Caculate & constrain translate & rotate
      translatePower = translatePID.calculateOutput(distanceToTarget);
      rotatePower = turnPID.calculateOutput(angleError);

      translatePower *= moveScaleFactor;

      translatePower = constrainValue(translatePower, std::fabs(moveScaleFactor) * maxTranslatePower, -std::fabs(moveScaleFactor) * maxTranslatePower);
      rotatePower = constrainValue(rotatePower, maxRotatePower, -maxRotatePower);
    }
    else { // We are basically on the point, so turn to final orientation
      translatePower = 0;
      rotatePower = 0;

      orientationError =  endOrientation - (globalPose.theta * RAD_TO_DEG);
      orientationError = constrainAngle180(orientationError);
      
      orientPower = turnPID.calculateOutput(orientationError);
      orientPower = constrainValue(orientPower, maxOrientPower, -maxOrientPower);
      
      if(std::fabs(orientationError) <= 2) {
        orientPower = 0;
        moveFinished = true;
      }
    }

    // If near the target, stop turning
    if(std::fabs(distanceToTarget) <= 1) {
      rotatePower = 0;
      settledCount++;
    }
    else settledCount = 0;

    if(settledCount > 4) {
      onTarget = true;
    }

    // Exit conditions
    if(moveFinished || toPointTimer >= maxTime) {
      leftDrive.stop(hold);
      rightDrive.stop(hold);
      break;
    }

    // Brain.Screen.clearScreen(black);
    Brain.Screen.setCursor(1, 1);
    Brain.Screen.print( "Drive Error: %4.1f", distanceToTarget);
    Brain.Screen.setCursor(1, 22);
    Brain.Screen.print("Drive Power: %3.1f", translatePower);
    Brain.Screen.setCursor(2, 1);
    Brain.Screen.print( "Turn Error: %4.1f", angleError);
    Brain.Screen.setCursor(2, 22);
    Brain.Screen.print( "TurnPower: %3.1f", rotatePower);

    // setDrive(translatePower + rotatePower + orientPower, translatePower - rotatePower - orientPower);
    leftDrive.spin(forward, translatePower + rotatePower + orientPower, volt);
    rightDrive.spin(forward, translatePower - rotatePower - orientPower, volt);
    
    wait(20, msec);
  }
  drivePID.reset();
  turnPID.reset();
}
// void MoveToPoint(double targetX, double targetY, double endOrientation, 
//                 double maxTranslatePower, double maxRotatePower, double maxOrientPower, int maxTime) {
//   toPointTimer.reset();
//   toPointTimer.time(msec);
//   double distanceToTarget, angleToTarget, orientationError;
//   double translatePower, rotatePower, xError, yError, movementScaleFactor;
//   double orientPower = 0;
//   int settledCount = 0;
//   bool onTarget = false;
//   bool moveFinished = false;
//   while(true) {
//     xError = targetX - globalPose.x;
//     yError = targetY - globalPose.y;

//     // Get errors
//       distanceToTarget = sqrt(pow(xError, 2) + pow(yError, 2));
//       angleToTarget = (atan2(xError, yError) - globalPose.theta) * RAD_TO_DEG;

//       angleToTarget = constrainAngle180(angleToTarget);

//     // Prioritize turning 
//     // Larger angle = less drive power due to cos values; as angle error approaches 0, scale factor approaches 1
//     movementScaleFactor = cos((constrainAngle90(angleToTarget)) * DEG_TO_RAD); 
    
//     // angleToTarget = constrainAngle90(angleToTarget);

//     if(onTarget == false) { // We still need to approach the point
//       // Caculate & constrain translate & rotate
//       translatePower = translatePID.calculateOutput(distanceToTarget);
//       rotatePower = turnPID.calculateOutput(angleToTarget);

//       translatePower *= movementScaleFactor;

//       translatePower = constrainValue(translatePower, std::fabs(movementScaleFactor) * maxTranslatePower, -std::fabs(movementScaleFactor) * maxTranslatePower);
//       rotatePower = constrainValue(rotatePower, maxRotatePower, -maxRotatePower);
//     }
//     else { // We are basically on the point, so turn to final orientation
//       translatePower = 0;
//       rotatePower = 0;

//       orientationError =  endOrientation - (globalPose.theta * RAD_TO_DEG);
//       orientationError = constrainAngle180(orientationError);
//       orientPower = turnPID.calculateOutput(orientationError);
//       orientPower = constrainValue(orientPower, maxOrientPower, -maxOrientPower);
      
//       if(std::fabs(orientationError) <= 2) {
//         orientPower = 0;
//         moveFinished = true;
//       }
//     }

//     // If near the target, stop turning
//     if(std::fabs(distanceToTarget) <= 1) {
//       rotatePower = 0;
//       settledCount++;
//     }
//     else settledCount = 0;

//     if(settledCount > 4) {
//       onTarget = true;
//     }

//     // Exit conditions
//     if(moveFinished || toPointTimer >= maxTime) {
//       leftDrive.stop(hold);
//       rightDrive.stop(hold);
//       break;
//     }

//     // Brain.Screen.clearScreen(black);
//     Brain.Screen.setCursor(1, 1);
//     Brain.Screen.print( "Drive Error: %4.1f", distanceToTarget);
//     Brain.Screen.setCursor(1, 22);
//     Brain.Screen.print("Drive Power: %3.1f", translatePower);
//     Brain.Screen.setCursor(2, 1);
//     Brain.Screen.print( "Turn Error: %4.1f", angleToTarget);
//     Brain.Screen.setCursor(2, 22);
//     Brain.Screen.print( "TurnPower: %3.1f", rotatePower);

//     // setDrive(translatePower + rotatePower + orientPower, translatePower - rotatePower - orientPower);
//     leftDrive.spin(forward, translatePower + rotatePower + orientPower, volt);
//     rightDrive.spin(forward, translatePower - rotatePower - orientPower, volt);
    
//     wait(20, msec);
//   }
//   drivePID.reset();
//   turnPID.reset();
// }

double invertAngle(double input) {
  input *= -1;
  input += 360;
  return input;
}
double mapDegToPositiveValues(double input) {
  if(input < 0) {
    input += 360;
  }
  return input;
}




// timer toPointTimer;
// void MoveToPoint(double targetX, double targetY, double endOrientation, 
//                 double maxTranslatePower, double maxRotatePower, double maxOrientPower, int maxTime) {
//   toPointTimer.reset();
//   toPointTimer.time(msec);
//   double distanceToTarget, angleToTarget, orientationError, atanValue, invertedAngle;
//   double translatePower, rotatePower, xError, yError, movementScaleFactor;
//   double orientPower = 0;
//   int settledCount = 0;
//   bool onTarget = false;
//   bool moveFinished = false;
//   while(true) {
//     xError = targetX - globalPose.x;
//     yError = targetY - globalPose.y;
//     // Get errors
//       distanceToTarget = sqrt(pow(xError, 2) + pow(yError, 2));

//       atanValue = mapDegToPositiveValues(atan2(yError, xError) * RAD_TO_DEG);
//       invertedAngle = invertAngle(globalPose.theta * RAD_TO_DEG);

//       angleToTarget = constrainAngle180(atanValue - invertedAngle);

//       // if(backwards) {
//       //   distanceToTarget *= -1;
//       //   angleToTarget += 180;
//       // }
//       // angleToTarget = constrainAngle180(angleToTarget);

//     // Prioritize turning: Larger angle = less drive pow due to cos values,
//     // scale factor approaches 1 as angle error approaches 0
//     movementScaleFactor = cos(constrainAngle90(angleToTarget) * DEG_TO_RAD); 
    
//     if(onTarget == false) { // We still need to approach the point
//       // Caculate translate & rotate power
//       translatePower = translatePID.calculateOutput(distanceToTarget);
//       rotatePower = turnPID.calculateOutput(angleToTarget);

//       translatePower *= movementScaleFactor;

//       translatePower = constrainValue(translatePower, std::fabs(movementScaleFactor) * maxTranslatePower, -std::fabs(movementScaleFactor) * maxTranslatePower);
//       rotatePower = constrainValue(rotatePower, maxRotatePower, -maxRotatePower);
//     }
//     else { // We are basically on the point, so turn to final orientation
//       translatePower = 0.0;
//       rotatePower = 0.0;

//       orientationError =  endOrientation - (globalPose.theta * RAD_TO_DEG);
//       orientationError = constrainAngle180(orientationError);

//       orientPower = turnPID.calculateOutput(orientationError);
//       orientPower = constrainValue(orientPower, maxOrientPower, -maxOrientPower);
      
//       if(std::fabs(orientationError) <= 2) {
//         orientationError = 0;
//         moveFinished = true;
//       }
//     }

//     // If near the target, stop turning
//     if(std::fabs(distanceToTarget) <= 1) {
//       rotatePower = 0;
//       settledCount++;
//     }
//     else settledCount = 0;

//     if(settledCount > 4) {
//       onTarget = true;
//     }

//     // Exit conditions
//     // if(moveFinished || toPointTimer >= maxTime) {
//     //   leftDrive.stop(hold);
//     //   rightDrive.stop(hold);
//     //   break;
//     // }

//     // Brain.Screen.clearScreen(black);
//     Brain.Screen.setCursor(1, 1);
//     Brain.Screen.print( "Drive Error: %4.1f", distanceToTarget);
//     Brain.Screen.setCursor(1, 22);
//     Brain.Screen.print("Drive Power: %3.1f", translatePower);
//     Brain.Screen.setCursor(2, 1);
//     Brain.Screen.print( "Turn Error: %4.1f", angleToTarget);
//     Brain.Screen.setCursor(2, 22);
//     Brain.Screen.print( "TurnPower: %3.1f", rotatePower);

    // setDrive(translatePower + rotatePower + orientPower, translatePower - rotatePower - orientPower);
    // leftDrive.spin(forward, translatePower + rotatePower + orientPower, volt);
    // rightDrive.spin(forward, translatePower - rotatePower - orientPower, volt);
    
//     wait(20, msec);
//   }
//   drivePID.reset();
//   turnPID.reset();
// }

// broken move function //
// timer moveTimer;
// void Move(double driveTarget, double maxDrivePower, double turnTarget, double maxTurnPower, 
//           int maxTime, bool driveSlew, bool turnSlew) {
//   // Reset drive encoders
//   frontEnc.resetPosition();
//   // Restart timer
//   moveTimer.reset();
//   moveTimer.time(msec);
//   // Convert target from inch to deg
//   driveTarget *= 360 / (2.75 * M_PI);
//   // Initialize slew
//   double driveSlewRate = 0.0;
//   double driveTempMax = 0.0;
//   double turnSlewRate = 0.0;
//   double turnTempMax = 0.0;
//   double slewStart = 5;
//   bool dSlew = false;
//   bool tSlew = false;
//   bool moveSettled = false;
//   // Slew conditions 
//   if(std::fabs(driveTarget) >= 18 && driveSlew) {
//     dSlew = true;
//     driveTempMax = slewStart;
//     driveSlewRate = maxDrivePower / 100; 
//   }
//   if(std::fabs((Inertial.rotation() - turnTarget)) > 90 && turnSlew) {
//     tSlew = true;
//     turnTempMax = slewStart;
//     turnSlewRate = maxTurnPower / 60;
//   }
//   // Initialize variables
//   double drivePosition, driveHeading, driveError, turnError, drivePower, turnPower;
//   while(true) {
//     drivePosition = frontEnc.position(deg);
//     driveHeading = Inertial.rotation(deg);

//     // Define errors
//     driveError = driveTarget - drivePosition;
//     turnError = constrainAngle180(turnTarget - driveHeading);
//     // Calculate and constrain power
//     drivePower = drivePID.calculateOutput(driveError);
//     turnPower = drivePID.calculateOutput(turnError);
    
//     drivePower = constrainValue(drivePower, maxDrivePower, -maxDrivePower);
//     turnPower = constrainValue(turnPower, maxTurnPower, -maxTurnPower);
    
//     // Drive slew
//     if (dSlew) {
//       // Increment max power by slew rate
//       driveTempMax += driveSlewRate;
//       // Cap max power
//       driveTempMax = constrainValue(driveTempMax, maxDrivePower, -maxDrivePower);
//       // Constrain drive power to max slew power
//       drivePower = constrainValue(drivePower, driveTempMax, -driveTempMax);
//     }
//     else {
//     // Constrain drive power if not using slew rate
//     drivePower = constrainValue(drivePower, maxDrivePower, -maxDrivePower);
//     }

//     // Turn Slew 
//     if (tSlew) {
//       // Increment max turn power by slew rate
//       turnTempMax += turnSlewRate;
//       // Cap max power
//       turnTempMax = constrainValue(turnTempMax, maxTurnPower, -maxTurnPower);
//       // Constrain drive power to max slew power
//       turnPower = constrainValue(turnPower, turnTempMax, -turnTempMax);
//     }
//     else {
//     // Constrain turn power if not using slew rate
//     turnPower = constrainValue(turnPower, maxTurnPower, -maxTurnPower);
//     }

//     // Exit conditions
//     if(std::fabs(driveError) <= 5 && std::fabs(turnError) <= 5){
//       moveSettled = true;
//     }
//     if(std::fabs(turnError) <= 5 && driveTarget == 0){
//       moveSettled = true;
//     }
//     if (moveTimer >= maxTime){
//       moveSettled = true;
//     }

//     if(moveSettled) {
//       leftDrive.stop(brake);
//       rightDrive.stop(brake);
//       drivePower = 0;
//       turnPower = 0;
//       break;
//     }

// // Print Debug Info
//     Brain.Screen.clearScreen(black);
//     Brain.Screen.setCursor(10, 1);
//     Brain.Screen.print( "Drive Error: %4.1f", driveError);
//     Brain.Screen.setCursor(10, 22);
//     Brain.Screen.print("Power/Max: %3.1f / %3.1f", drivePower, maxDrivePower);
//     Brain.Screen.setCursor(11, 1);
//     Brain.Screen.print( "Turn Error: %4.1f", turnError);
//     Brain.Screen.setCursor(11, 22);
//     Brain.Screen.print( "Power/Max: %3.1f / %3.1f", turnPower, maxTurnPower);
//     Brain.Screen.setCursor(12, 1);
//     Brain.Screen.print( "Slew/Max: %1.2f/%2.1f", driveSlewRate, driveTempMax);
//     Brain.Screen.setCursor(12, 22);
//     Brain.Screen.print( "Turn/Max: %1.2f/%2.1f", turnSlewRate, turnTempMax);
    
//     // Output to motors
//     setDrive(drivePower + turnPower, drivePower - turnPower);
//     wait(20, msec);
//   }
//   drivePID.reset();
//   turnPID.reset();
//   moveTimer.reset();
// }