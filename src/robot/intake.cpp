#include "robot_header/intake.h"
#include <string>

// Intake State Machine
// class Intake {
//   private:
//     std::string intakeCurrentState;
//     enum intakeState{INTAKING, OUTTAKING, JAMMED};

//   public:
//   void setState(intakeState s) {
//     intakeCurrentState = s;
//   }
// };

// Intake newIntake;
// void testIntakeState() {
//   newIntake.setState(OUTTAKING);
// }

// Prototype state machine
// std::string currentIntakeState;
// enum newIntakeState{INTAKING, OUTTAKING, JAMMED, OFF};

// void setIntakeState(newIntakeState s) {
//   currentIntakeState = s;
// }

// int intakeStateMachine() {
//   while(true){
//     if(currentIntakeState == "INTAKING") {
//       intake.spin(forward, 13, volt);
//     }
//     if(currentIntakeState == "OUTTAKING") {
//       intake.spin(reverse, 13, volt);
//     }
//     if(currentIntakeState == "JAMMED") {
//       intake.spin(reverse, 13, volt);
//       wait(500, msec);
//     }
//     if(currentIntakeState == "OFF") {
//       intake.stop(coast);
//     }
//   }
//   return(0);
// }


// Drive control function
void intakeControl() {
  double intakePower = 12.7 * (Controller.ButtonR2.pressing() - Controller.ButtonR1.pressing());
  intake.spin(forward, intakePower, volt);
}

// Shooting functions
void shootDisc(int msecDelay) {
  intake.spin(reverse, 13, volt);
  wait(msecDelay, msec);
  intake.stop(coast);
}
void singleShot() {
  shootDisc(120); // 148
}
void shoot(int discNum, int msecDelay) {
  for(int i=0; i < discNum-1; i++) {
    singleShot();
    wait(msecDelay, msec);
  }
  shootDisc(500); // accounts for slower index on last disc
}
void tripleShot() {
  shootDisc(1000);
}

// Intake toggle
bool intConHeld = false;
double intVelo = 0;
int intakeToggle() {
  while(true) {
	if(Controller.ButtonR2.pressing()) {
		if(!intConHeld){
			//If not right speed
			if(intVelo != -13) {
				intVelo = -13; 
			}
			else { 
        intVelo = 0;
      }
			intConHeld = true;
		}
  }
  else {
    intConHeld = false;
  }
  

  if(Controller.ButtonL2.pressing()) {
    intVelo = -13;
  }
  else if(intVelo != -13) {
    intVelo = 0;
  }

  intake.spin(forward, intVelo, volt);

  wait(20, msec);

  }
  return(0);
}

int autoEject() {
  double intakeVelocity;
  int intakeRunningCounter = 0.0;
  int intakeJammedCounter = 0.0;
  bool intakeOn = false;
  bool intakeJammed = false;
  while(true) {
    intakeVelocity = intake.velocity(rpm);

    // Delay / counter so that starting intake won't immediately reverse
    if(intake.voltage() >= 1 ) {
      intakeRunningCounter++;
    }
    else intakeRunningCounter = 0;

    // Check if intake is supposed to be on
    if(intakeRunningCounter >= 50) {
      intakeOn = true;
    }
    else intakeOn = false;

    // Define jammed bool
    if(intakeOn) {
      if(intakeJammedCounter >= 30) {
        intakeJammed = true;
      }
      else intakeJammed = false;
    }
    else intakeJammed = false;

    if(intakeOn && intake.velocity(rpm) <= 400) {
      intakeJammedCounter++;
    }
    else intakeJammedCounter = 0.0;

    // Reverse intake if jammed 
    if(intakeJammed) {
      intake.spin(reverse, 13, volt);
      wait(750, msec);
      intake.spin(forward, 13, volt);
      intakeJammed = false;
    }

    wait(20, msec);
  }

  return(0);
}
