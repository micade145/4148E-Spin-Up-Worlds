#include "vex.h"

using namespace vex;
using signature = vision::signature;

brain Brain;
controller Controller = controller(primary);

// Drivetrain
motor rFront = motor(PORT8, ratio6_1, false);
motor rBack = motor(PORT9, ratio6_1, false);
motor rTop = motor(PORT10, ratio6_1, true);
motor lFront = motor(PORT18, ratio6_1, true);
motor lBack = motor(PORT19, ratio6_1, true);
motor lTop = motor(PORT20, ratio6_1, false);
motor_group leftDrive(lFront, lBack, lTop);
motor_group rightDrive(rFront, rBack, rTop);

// Subsystems
motor flywheel = motor(PORT11, ratio6_1, true);
motor intake = motor(PORT7, ratio6_1, false);

// Sensors
inertial Inertial = inertial(PORT5);
rotation sideEnc = rotation(PORT5, false);
rotation frontEnc = rotation(PORT6, false);
// Vision sensor
signature Vision__BLUE_GOAL = signature (1, -3039, -2023, -2531, 11703, 13895, 12799, 6.4, 0);
signature Vision__RED_GOAL = signature (2, 11205, 12867, 12036, -1735, -837, -1286, 6.3, 0);
vision Vision = vision(PORT1, 64, Vision__BLUE_GOAL, Vision__RED_GOAL);


// Pneumatics
digital_out angler = digital_out(Brain.ThreeWirePort.A);
digital_out indexer = digital_out(Brain.ThreeWirePort.B);
// Endgame
digital_out leftEndgame = digital_out(Brain.ThreeWirePort.G);
digital_out rightEndgame = digital_out(Brain.ThreeWirePort.H);
digital_out bottomEndgame = digital_out(Brain.ThreeWirePort.F);

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 *
 * This should be called at the start of your int main function.
 */
void vexcodeInit(void) {
  // Nothing to initialize
}