using namespace vex;
using signature = vision::signature;

extern brain Brain;
extern controller Controller;

// Drivetrain
extern motor rFront;
extern motor rBack; 
extern motor rTop;
extern motor lFront;
extern motor lBack;
extern motor lTop;
extern motor_group leftDrive;
extern motor_group rightDrive;

// Subsystems
extern motor flywheel;
extern motor intake;

// Sensors
extern inertial Inertial;
extern rotation sideEnc;
extern rotation frontEnc;
extern signature Vision__BLUE_GOAL;
extern signature Vision__RED_GOAL;
extern vision Vision;

// Pneumatics
extern digital_out angler;
// extern digital_out indexer;
// Endgame
extern digital_out leftEndgame;
extern digital_out rightEndgame;
extern digital_out bottomEndgame;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 *
 * This should be called at the start of your int main function.
 */
void vexcodeInit(void);
