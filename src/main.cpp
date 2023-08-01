/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       VEX                                                       */
/*    Created:      Thu Sep 26 2019                                           */
/*    Description:  Competition Template                                      */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"
#include "my_lib_header/autoselector.h"
using namespace vex;

competition Competition;

void printAt(int mLine, int mColumn, vex::color mColor, std::string mText) {
  Brain.Screen.clearLine(mLine);
  Brain.Screen.setCursor(mLine, mColumn);
  Brain.Screen.setPenColor(mColor);
  Brain.Screen.print(mText.c_str());
}

void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  // enableSelect = true;
  
  while(Inertial.isCalibrating()) {
    wait(20, msec);
    printAt(1, 10, red, "Inertial is Calibrating - WAIT");
    // Brain.Screen.setCursor(1, 1);
    // Brain.Screen.setPenColor(red);
    // Brain.Screen.print("Inertial is Calibrating - Wait");
  }
  Brain.Screen.setPenColor(white);
  Brain.Screen.clearScreen();
}

// Tasks
task fwControl = task(flywheelControl);
// task trackPos = task(trackPosition); // odom
// task fwBangBang = task(flywheelBangBang);  // flywheel control loop
// task ejectFourthDisc = task(autoEject);  // unfinished auto 4th disc eject

void autonomous(void) {
  resetOdomSensors();
  enableSelect = false;

  // //// Run Selected Auto ////
  if (autonToRun == 0) { // Button 1
    leftWP();
    // progSkills();
    // Move(24, 8, 0, 0, 800, true, false);
    // Move(0, 0, 90, 10, 800, false, false);
    // Move(0, 0, 180, 10, 800, false, true);
  }
  if (autonToRun == 1) { // Button 2
    rightWP();
  }
  if (autonToRun == 2) { // Button 3
    deviousRightWP();
  }
  if (autonToRun == 3) { // Button 4
    soloWP();
  }
  if (autonToRun == 4) { // Button 5
    
  }
  if (autonToRun == 5) {
    progSkills();
  }
}


void usercontrol(void) {
  Brain.Screen.clearScreen(black);
  flywheelTarget = FW_VOLT_LOW; // 10
  angler.set(true);
  enableSelect = false;

  // Initialize tasks
  // task fwToggle = task(flywheelToggle);
  task visionTracking = task(alignToGoalTask);

  // double turnPower = 0.0;

  // Running prog in driver
  // task prog = task(progSkills);
  // waitUntil(Controller1.Axis3.value() >= 50);
  // task(betterProgSkills).stop();
  while(true) {
    // Drive
    splitArcadeVolt();
    // setDrive(driveForwardPower + turnPower, driveForwardPower - turnPower);

    // Vision tracking
    while(Controller.ButtonL2.pressing()) {
      enableTracking = true;
    }
    enableTracking = false;

    // if(Controller.ButtonL1.pressing()) {
    //   trackGoal();
    //   turnPower = visionTurnPower;
    // }
    // else {
    //   turnPower = driveTurnPower;
    // }

    // Intake
    intakeControl();

    // Alliance color toggle
    // Controller.ButtonX.pressed(toggleAllianceColor);
    // Brain.Screen.setCursor(12, 1);
    // isRed ? Brain.Screen.print("We are RED") : Brain.Screen.print("We are BLUE");
    
    // Flywheel
    
    // Angler
    Controller.ButtonL1.pressed(toggleAngler);
    
    // Indexer
    // Controller.ButtonR1.pressed(singleShot);
    
    // Endgame
    Controller.ButtonDown.pressed(toggleBottomEndgame);  // leftmost scuff
    Controller.ButtonRight.pressed(toggleLeftEndgame);  // left mid scuff
    Controller.ButtonY.pressed(toggleRightEndgame);     // right mid scuff
    Controller.ButtonB.pressed(toggleAllEndgame);       // rightmost scuff

    wait(20, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
  }
}


// Main will set up the competition functions and callbacks.
int main() {
  // resetDrivePosition();
  // resetOdomSensors();
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  angler.set(false);
  leftEndgame.set(false);
  rightEndgame.set(false);
  bottomEndgame.set(false);

  // Run the pre-autonomous function.
  enableSelect = true;
  while(enableSelect) {
    // Alliance Color Buttons
    redAlliance.render();
    blueAlliance.render();

    if(redAlliance.isClicked()) {
      isRed = true;
      redAlliance.buttonColor = vex::red;
      blueAlliance.buttonColor = vex::black;
    }
    if(blueAlliance.isClicked()) {
      isRed = false;
      blueAlliance.buttonColor = vex::blue;
      redAlliance.buttonColor = vex::black;
    }

    // Iterate through the button array - 6 buttons
    for(int i = 0; i < 6; i++) {
      autonButtons[i].render();

      if(autonButtons[i].isClicked()){ // If button pressed
        for(int j = 0; j < 6; j++) {
        // Clear all button colors
          autonButtons[j].buttonColor = vex::black;
        }
    
        autonToRun = i; // Set pressed button as autonToRun
        autonButtons[i].buttonColor = vex::color(255,145,175); // cool pink rgb values
        
        // Output Auton name
        Brain.Screen.clearLine(1);
        Brain.Screen.setCursor(1 , 1);
        Brain.Screen.print(autonButtons[i].autoName.c_str());
      }
      wait(20, msec);  // auton buttons loop timeout
    }
      wait(20, msec); // main loop timeout
  }

  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}
