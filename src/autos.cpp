#include "autos.h"

//// Ben bot autos ////

// Test autos
void bexTest() { // Box test using Move function
  Move(24, 8, 0, 10, 1000, true, false);
  wait(250, msec);
  Move(0, 0, 90, 10, 1000, false, false);
  wait(250, msec);
  Move(24, 8, 90, 10, 1000, true, false);
  wait(250, msec);
  Move(0, 0, 180, 10, 1000, false, false);
  wait(250, msec);
  Move(24, 8, 180, 10, 1000, true, false);
  wait(250, msec);
  Move(0, 0, 270, 10, 1000, false, false);
  wait(250, msec);
  Move(24, 8, 270, 10, 1000, true, false);
  wait(250, msec);
  Move(0, 0, 0, 10, 1000, false, false);
}

void odomBoxTest() { // Box test using odom
  // resetOdomSensors();
  globalPose.setPoint(48, 24, 0);
  wait(250, msec);
  MoveToPoint(48, 48, 90, 8, 10, 10, 1500);
  wait(250, msec);
  MoveToPoint(72, 48, 180, 8, 10, 10, 1500);
  wait(250, msec);
  MoveToPoint(72, 24, 270, 8, 10, 10, 1500);
  wait(250, msec);
  MoveToPoint(48, 24, 0, 8, 10, 10, 1500);
}

void odomReturnToStartTest() {
  globalPose.setPoint(48, 24, 90);

  MoveToPoint(80, 80, 225, 7, 10, 10, 2000);
  wait(5, sec);
  MoveToPoint(0, 0, 90, 7, 10, 10, 2000);
  wait(100, sec);
}

void flywheelTest() { // Infinte flywheel loop to test control loop
  flywheelTarget = 10;
  while(true) {
    wait(20, msec);
  }
}


// Match autos
void leftWP() { // shoot 7
  intake.spin(forward, 13, volt);
  flywheelTarget = 13;
  
  // Grab 3rd disc
  Move(-5, 4, 0, 3, 100, false, false);
  Move(8, 8, 318, 10, 750, false, false);
  Move(-5.5, 5, 318, 6, 700, false, false);

  // Move near mid and shoot
  Move(0, 0, 45, 8, 600, false, false);
  Move(16.8, 10, 45, 9, 800, true, false);
  intake.stop(coast);
  Move(0, 0, 348, 10, 750, false, false); // turn 350
  thread moveForward = thread([] {Move(5, 6, 348, 10, 800, false, false);});
  thread(moveForward).detach();
  wait(450, msec);
  shoot(3, 800);

  // Grab 3 stack 
  intake.spin(forward, 12.5, volt);
  // Move(-15, 8, 46, 7, 850, false, false); // turn 47
  Move(-8, 6, 348, 8, 800, false, false);
  Move(0, 0, 46, 10, 800, false, false);
  wait(50, msec);
  flywheelTarget = 11;
  Move(30, 5.5, 46, 8, 1500, true, false);  // move 30, 6 volt
  wait(350, msec);
  
  // Back up, turn, and shoot 3 discs
  Move(-7, 8, 46, 10, 600, false, false); // added back up to not hit 7110A
  Move(0, 0, 336, 10, 800, false, false); // 336 
  intake.stop(coast);
  Move(5, 8, 336, 8, 800, false, false); // 336
  shoot(3, 600);

  // Grab one more disc and shoot 
  intake.spin(forward, 13, volt);
  Move(5.4, 10, 336, 8, 500, false, false);
  wait(800, msec);
  intake.stop(coast);
  shootDisc(700);
}

void rightWP() {  // shoot 7
  flywheelTarget = 13;
  intake.spin(forward, 13, volt);

  // Grab third disc
  Move(21, 8, 350, 10, 900, true, false);
  wait(100, msec);

  // Get roller and drive near mid
  Move(-15.25, 8, 271, 12, 900, true, false); // drive 47
  wait(75, msec);
  Move(15, 6, 227, 10, 800, false, false);  // dirve 16.2

  // Shoot 3 
  intake.stop(coast);
  Move(0, 0, 293, 10, 700, false, false); // turn 295
  thread moveForward = thread([] {Move(9.3, 6, 293, 8, 800, false, false);});
  thread(moveForward).detach();
  flywheelTarget = 12.5;
  wait(800, msec);
  shoot(3, 825);

  // Get row of 3 discs
  flywheelTarget = 10.3;  // 10.6
  intake.spin(forward, 13, volt);
  Move(-6.3, 9, 293, 7, 700, false, false); // move -8, turn 295
  Move(0, 0, 227, 10, 800, false, false);
  Move(70, 6, 227, 10, 1550, true, false);  // move 70, turn 226
  wait(250, msec);
  Move(-19, 6, 227, 10, 800, false, false); // move -18, turn 226

  // Turn and shoot 3 discs
  Move(0, 0, 309, 10, 700, false, false); // turn 310
  intake.stop(coast);
  Move(8, 6, 309, 10, 800, false, false); // turn 310
  shoot(3, 750);
  flywheelTarget = 10;  // 10.6
  intake.spin(forward, 13, volt);

  // Grab last disc and shoot 
  Move(3, 5, 309, 8, 500, false, false);
  wait(800, msec);
  intake.stop(coast);
  shootDisc(750);
}

void deviousRightWP() { // shoot 7 and knocks over 3 stack
  flywheelTarget = 13; // 12.4
  intake.spin(forward, 13, volt);

  // Grab third disc 
  Move(19, 8, 310, 12, 800, false, false);
  wait(50, msec);
  // wait(150, msec);

  // Get roller and drive near mid
  Move(-21, 8, 225, 10, 960, false, false); // drive 55, 7, turn 225, 9
  wait(200, msec);
  Move(17, 6, 225, 10, 800, false, false);  // drive 18

  // Shoot 3 
  Move(0, 0, 295, 10, 700, false, false); // turn 293
  intake.stop(coast);
  thread moveForward = thread([] {Move(9, 6, 295, 8, 800, false, false);});
  thread(moveForward).detach();
  wait(700, msec);
  shoot(3, 800);

  // Get row of 3 discs
  flywheelTarget = 10.4;  // 10.6
  intake.spin(forward, 13, volt);
  moveForward.interrupt();
  // Move(-10, 8, 226, 7, 750, false, false); // old curve turn
  Move(-8, 7, 295, 10, 900, false, false);
  Move(0, 0, 228, 10, 800, false, false); // turn 226
  Move(66, 5, 228, 10, 1800, true, false);  // drive 68, 6 volt
  wait(150, msec);
  Move(-15, 7, 228, 10, 800, false, false); // Move -11, turn 225

  // Turn and shoot 3 discs
  Move(0, 0, 310, 10, 750, false, false); // 310
  Move(6, 8, 310, 10, 800, false, false);  // 310
  intake.stop(coast);
  shoot(3, 700);

  // Grab last disc and shoot 
  intake.spin(forward, 13, volt);
  Move(5, 6, 310, 10, 800, false, false);  // 310
  wait(600, msec);
  intake.spin(reverse, 13, volt);
}

void soloWP() { // 6 discs, 2 rollers
  flywheelTarget = 13; // 11.1 v
  intake.spin(forward, 13, volt);

  // Grab 3rd disc
  Move(-5, 4, 0, 3, 100, false, false);
  Move(8, 8, 318, 10, 750, false, false);
  Move(-5.5, 5, 318, 6, 800, false, false);
  wait(50, msec);

  // Move near mid and shoot
  Move(0, 0, 45, 8, 700, false, false);
  Move(16.8, 10, 45, 9, 800, true, false);
  intake.stop(coast);
  Move(0, 0, 350, 10, 750, false, false);
  thread moveForward = thread([] {Move(7, 6, 350, 10, 800, false, false);});
  thread(moveForward).detach();
  flywheelTarget = 12.5;
  wait(550, msec);
  shoot(3, 850);

  
  // Move(-15, 8, 47, 6.6, 850, false, false); // curve turn
  Move(-8, 6, 350, 8, 800, false, false);
  Move(0, 0, 47, 10, 800, false, false);
  intake.spin(forward, 13, volt);
  wait(50, msec);
  flywheelTarget = 11;

  // Move(20, 10, 47, 10, 400, false, false);
  // wait(150, msec);
  // Move(28, 6, 47, 10, 850, false, false);
  Move(32, 5, 47, 8.1, 1500, true, false);  // move 34, 5.5
  wait(275, msec);

  // Turn and shoot 3 discs
  Move(0, 0, 331, 10, 700, false, false);
  intake.stop(coast);
  wait(150, msec);
  shoot(3, 600);

  // Go to roller while intaking row of 3 discs
  intake.spin(forward, 13, volt);
  Move(0, 0, 45, 10, 800, false, false);  // turn 47
  Move(70, 12, 45, 10, 1900, false, false); //move 68, 1800 timeout

  // Turn and spin roller
  Move(0, 0, 270, 10, 700, false, false);
  Move(-12, 8, 270, 10, 830, false, false);
  wait(150, msec);
  Move(5, 5, 270, 8, 800, false, false);
}

// void nineDiscSoloWP() { // 9 discs, 2 rollers
//   flywheelTarget = 13; // 11.1 v
//   intake.spin(forward, 13, volt);

//   // Grab 3rd disc
//   Move(-5, 4, 0, 3, 100, false, false);
//   Move(8, 8, 318, 10, 750, false, false);
//   Move(-25, 6, 225, 9, 1000, false, false);
//   wait(50, msec);

//   // Move near mid and shoot
//   intake.stop(coast);
//   Move(0, 0, 345, 10, 750, false, false);
//   // thread moveForward = thread([] {Move(7, 7, 350, 10, 800, false, false);});
//   // thread(moveForward).detach();
//   // flywheelTarget = 12.5;
//   wait(200, msec);
//   shoot(3, 300); // 1596 msec @ 400, 996 msec @ 100

//   // Move(-8, 8, 350, 9, 800, false, false);
//   Move(0, 0, 47, 10, 700, false, false);
//   intake.spin(forward, 13, volt);
//   flywheelTarget = 12; // 11
//   wait(50, msec);

//   // Move(20, 10, 47, 10, 400, false, false);  // old 
//   // wait(100, msec);                          //  
//   // Move(28, 6, 47, 10, 850, false, false);   //

//   Move(32, 5, 47, 8.1, 1500, true, false);  // move 34, 5.5
//   wait(100, msec);

//   // Turn and shoot 3 discs
//   Move(0, 0, 331, 10, 700, false, false);
//   intake.stop(coast);
//   wait(50, msec);
//   shoot(3, 300); // 1596 msec @ 400, 996 msec @ 100

//   // Go to roller while intaking row of 3 discs
// flywheelTarget = 13;
//   intake.spin(forward, 13, volt);
//   Move(0, 0, 45, 10, 700, false, false);  // turn 47
//   Move(70, 12.5, 45, 10, 1900, false, false); //move 68, 1800 timeout

//   // Turn and spin roller
//   Move(0, 0, 270, 10, 700, false, false);
//   Move(-12, 8, 270, 10, 900, false, false);
//   intake.stop(coast);
//   Move(8, 8, 281, 8, 800, false, false);
// shoot(3, 300); // 1596 msec @ 400, 996 msec @ 100

/*
 Max movement time + delay = 11,750 msec
 Shooting time: 796 msec per cycle + (delay * number of discs shooting)

 400 msec shot delay: 1596 * 3 = 4788
 Total time = 16,538 msec
 TIME TO SHAVE = 1538 msec
 
 100 msec shot delay: 996 * 3 = 2988
 Total time = 14,738 msec
 IN TIME: 262 msec left
*/
// }

void mysteryAuto() {  // does nothing (for now?)

}

int progSkills() { // UIL prog: 24 shots, 4 rollers, ~24 endgame 
  flywheelTarget = 10; // 8.8
  Inertial.setRotation(357, deg);
  wait(3.7, sec);

  // Shoot 2 preloads + 7 matchloads
  shoot(9, 850); // CHANGE TO (9, 850) LATER
  // Grab line of 3 discs from mid
  wait(50, msec);
  intake.spin(forward, 13, volt);
  flywheelTarget = 10.5;
  Move(0, 0, 275, 10, 800, false, false);
  Move(32, 8, 275, 10, 1200, true, false);
  wait(50, msec);
  Move(0, 0, 320, 10, 800, false, false); // turn 325
  wait(50, msec);
  Move(80, 6, 320, 10, 1600, true, false);  

  // Turn and shoot 3 discs 
  wait(120, msec);
  Move(0, 0, 83, 10, 900, false, false);  // 82
  wait(120, msec);
  intake.stop(coast);
  wait(50, msec);
  shoot(3, 850); // UNCOMMENT LATER

  // Get first roller 
  Move(0, 0, 129, 10, 800, false, false);
  intake.spin(forward, 13, volt);
  Move(-23.2, 6, 129, 10, 1000, true, false); // move -22, turn 130
  Move(0, 0, 180, 10, 800, false, false);
  Move(-7, 8, 180, 10, 700, false, false);
  wait(250, msec);

  // Get 3 stack and second roller
  Move(66.5, 4.5, 180, 12, 1700, true, false); // move 66, 5 v, turn 225
  wait(150, msec);
  Move(-9.8, 6, 180, 10, 800, false, false); // move -9.5
  Move(0, 0, 90, 10, 800, false, false);
  Move(-28.4, 8, 90, 10, 1080, false, false);  // -26
  wait(400, msec);
  Move(3.5, 6, 90, 10, 800, false, false);  // move 6

  // Align with second matchload station
  Move(0, 0, 192, 8, 800, false, false); // turn 192
  Move(33, 5.5, 192, 6, 1700, true, false);  // move 38
  flywheelTarget = 10; // 9.5
  wait(150, msec);
  Move(0, 0, 168, 7, 800, false, false); // turn 172
  intake.stop(coast);
  wait(500, msec);

  // Shoot 1 discs + 7 matchloads
  shoot(8, 800);  // CHANGE TO 8 LATER

  // Repeat of first part //  
  // Grab line of 3 discs from mid
  flywheelTarget = 10.5;
  Move(0, 0, 89, 10, 800, false, false);
  wait(50, msec);
  intake.spin(reverse, 13, volt);
  Move(32.5, 8, 89, 10, 1200, true, false); // move 39
  wait(50, msec);
  Move(0, 0, 133, 10, 800, false, false);
  wait(50, msec);
  intake.spin(forward, 13, volt);
  Move(80, 6, 133, 10, 1500, true, false);  // move 95
  wait(50, msec);

  // Turn and shoot 3 discs 
  Move(0, 0, 260, 10, 800, false, true);  // turn 262
  wait(100, msec);
  intake.stop(coast);
  wait(50, msec);
  shoot(3, 850);

  // Get 3rd roller
  flywheelTarget = 12.6;
  Move(0, 0, 315, 10, 900, false, false);
  intake.spin(forward, 13, volt);
  Move(-28.2, 7, 315, 10, 1200, true, false);
  wait(50, msec);
  Move(0, 0, 361, 10, 900, false, false);   // 1 or 361
  Move(-15, 8, 361, 10, 600, false, false); // 1 or 361
  wait(400, msec);

  // Get 4th roller and 1 disc
  Move(36, 6, 405, 12, 800, true, false);   // move 33, turn 45 or 405
  Move(0, 0, 630, 10, 800, false, false);   // 270 or 630
  Move(-27, 8, 630, 10, 800, true, false);  //270 or 630
  wait(400, msec);

  // hail mary last disc
  thread moveForward = thread([] {
    Move(14, 8, 630, 10, 800, false, false);  // Move 14, 270 or 630
  });
  thread(moveForward).detach();
  intake.stop(coast);
  wait(400, msec);
  shootDisc(800);

  // sus endgame 
  Move(0, 0, 675, 10, 800, false, false);   // 315 or 675
  Move(-5, 5, 675, 10, 800, false, false);  // 315 or 675
  wait(250, msec);
  toggleAllEndgame();
  return(0);
}


//// Archived autos :( ////

// void eightDiscSoloWP() { // 8 discs - deleted because not enough time
//   Inertial.setRotation(45, deg);
//   flywheelTarget = 13;
//   intake.spin(forward, 13, volt);

//   // Spin roller and shoot 2 preloads
//   Move(-10, 5, 45, 10, 120, false, false);
//   Move(20, 10, 45, 11, 850, false, false);
//   Move(0, 0, 343, 10, 700, false, false);
//   wait(500, msec);
//   shoot(2, 600);

//   // Get three discs from 3 stack
//   flywheelTarget = 10.5;
//   Move(0, 0, 45, 10, 600, false, false);
//   Move(20, 10, 45, 10, 400, false, false);
//   wait(150, msec);
//   Move(25, 6, 45, 10, 800, false, false); 
//   wait(150, msec);

//   // Turn and shoot 3 discs
//   Move(0, 0, 325, 10, 700, false, false);
//   wait(175, msec);
//   shoot(3, 600);

//   // Go to roller while row of 3 discs 
//   flywheelTarget = 10.8;
//   Move(0, 0, 44, 9, 800, false, false);
//   Move(80, 12, 44, 10, 1700, false, false);

//   // Turn and spin roller
//   Move(0, 0, 270, 10, 600, false, false);
//   Move(-10, 8, 270, 10, 500, false, false);
//   Move(5, 9, 280, 10, 700, false, false);
//   shoot(3, 600);
// }

// int progSkills() { // UIL prog: 24 shots, 4 rollers, ~24 endgame 
//   Flywheel.spin(forward, 8.8, volt);
//   Inertial.setRotation(357, deg);
//   wait(3, sec);

//   // Shoot 2 preloads + 7 matchloads
//     shoot(9, 1000);
//   // Grab line of 3 discs from mid
//     Intake.spin(forward, 13, volt);
//     Flywheel.spin(forward, 10, volt);
//     Move(0, 0, 275, 10, 800, false, false);
//     Move(38, 8, 275, 10, 1100, true, false);
//     Move(0, 0, 315, 10, 800, false, false);
//     Move(95, 6, 315, 10, 1500 true, false,);
//   // Turn and shoot 3 discs 
//     Move(0, 0, 87, 10, 800, false, false);
//     wait(250, msec);
//     shoot(3, 750);
//   // Get first roller 
//     Move(0, 0, 130, 10, 800, false, false);
//     Move(-27, 6, 130, 10, 800);
//     Move(0, 0, 180, 10, 800);
//     Move(-15, 8, 180, 10, 500);
//     wait(250, msec);
//   // Get second roller and 1 disc
//     Move(34, 6, 215, 12, 800);
//     Move(0, 0, 90, 10, 800);
//     Move(-2, 7, 90, 10, 900);
//     wait(250, msec);
//     Move(8, 6, 90, 10, 800);
//   // Align with second matchload station
//     Move(0, 0, 192, 10, 800);
//     Move(53, 7, 192, 10, 1400);
//     Move(0, 0, 172, 10, 800);
//     wait(500, msec);
//   // Shoot 1 discs + 7 matchloads
//     shoot(8, 950);
  
//             // Repeat of first part //  
//   // Grab line of 3 discs from mid
//     Flywheel.spin(forward, 9.7, volt);
//     Move(0, 0, 89, 10, 800);
//     Move(39, 8, 89, 10, 1100);
//     Move(0, 0, 133, 10, 800);
//     Move(95, 6, 133, 10, 1500);
//   // Turn and shoot 3 discs 
//     Move(0, 0, 265, 10, 800);
//     wait(250, msec);
//     shoot(3, 850);
//   // Get first roller

//     Move(0, 0, 315, 10, 800);
//     Move(-28, 5, 315, 10, 900);
//     Move(0, 0, 361, 10, 900);
//     Move(-15, 8, 361, 10, 500);

//     // Move(0, 0, 315, 10, 800);
//     // Move(-25, 5, 315, 10, 800);
//     // Move(0, 0, 4, 10, 800);
//     // Move(-15, 8, 4, 10, 800);

//     wait(250, msec);
//   // Get second roller and 1 disc
//     Flywheel.spin(forward, 11.6, volt);
//     Move(32, 6, 395, 12, 800);
//     Move(0, 0, 630, 10, 800);
//     Move(-27, 8, 630, 10, 800);
//     wait(250, msec);
//     // hail mary last disc
//     Move(18, 6, 630, 10, 800);
//     shoot(1, 0);
//     // endgame oh no
//     Move(0, 0, 675, 10, 800);
//     Move(-8, 5, 675, 10, 800);
//     wait(250, msec);
//     fireEndgame();
//     return(0);
// }

// int progSkills() { // states prog
//   resetOdomSensors();
//   intake.spin(forward, 13, volt);
//   flywheelTarget = 9.6;
//   globalPose.setPoint(31.5, 10.5, 0);

//   // Both rollers
//   Move(-10, 5, 0, 10, 300, false, false);
//   // MoveToPoint(14, 33, 90, 7, 10, 10, 1200); // (15, 33, 90)

//   // Move(35, 8, 327, 10, 1200, true, false); // curve 
//   // Move(-15, 8, 90, 10, 1100, false, false); // inconsistent curve
//   Move(29, 8, 323, 10, 900, true, false);
//   Move(0, 0, 90, 10, 800, false, false);
//   wait(150, msec);
//   Move(-20, 10, 90, 10, 800, false, false);
//   wait(150, msec);

//   // Move near mid and shoot 3
//   // MoveToPoint(23, 45, 0, 10, 5, 10, 1200); // (23, 45, 0)
//   Move(18, 7, 45, 10, 800, false, false);
//   Move(0, 0, 355, 10, 800, false, false);
//   wait(50, msec);

//   // Move(8, 6, 90, 10, 800, false, false);
//   shoot(3, 800);
//   flywheelTarget = 9.3;
//   // wait(1000, msec);

//   // Get mid three discs
//   Move(0, 0, 45, 10, 800, false, false); 
//   // MoveToPoint(55, 83, 320, 8, 6, 10, 1500); // (55, 83, -40/320)
//   Move(46, 8, 45, 10, 1600, true, false);
//   wait(250, msec);
//   Move(0, 0, 318, 10, 800, false, false);
//   wait(250, msec);
//   shoot(3, 400);
//   // wait(1000, msec);

//   flywheelTarget = 9.2;
//   // Get three from low goal 
//   Move(0, 0, 356, 10, 800, false, false);
//       // MoveToPoint(51, 128, 268, 6, 10, 10, 1500); // (51, 128, -92/268)
//   Move(70, 6, 356, 10, 1480, true, false);
//   wait(150, msec);

//   Move(0, 0, 252, 10, 800, false, false);
//   wait(200, msec);

//   // Line up to matchload station 1
//   Move(-18.5, 6, 252, 10, 1000, false, false); // -105
//   Move(0, 0, 270, 10, 800, false, false); // -93

//   shoot(10, 610); //  CHANGE TO 10 LATER!!!
//   // wait(1, sec);
//   flywheelTarget = 9.7;
  
//   Move(0, 0, 280, 10, 700, false, false);  // -70
//   Move(-38, 8, 280, 10, 1200, true, false);  // 285 or 290
//   wait(150, msec);
//   Move(0, 0, 180, 10, 800, false, false);

//   // Get roller
//   Move(-15, 8, 180, 10, 900, false, false);
//   // MoveToPoint(100, 92, 274, 8, 10, 10, 1200);  // -86
//   Move(46, 4.5, 180, 10, 1450, true, false);
//   wait(250, msec);

//   // Move(-60, 6, 270, 12, 1700, false, false); // curve not consistent?
//   Move(-8, 6, 180, 10, 500, false, false); // rely on time to get to right place
//   Move(0, 0, 270, 10, 800, false, false);
//   Move(-45, 6, 265, 10, 1300, true, false);
//   wait(100, msec);

//   // Move to mid and shoot 3
//   // MoveToPoint(110, 87, 180, 8, 10, 10, 1200); // (110, 87, 180)
//   Move(16, 6, 228, 10, 1000, false, false);
//   Move(0, 0, 173, 10, 800, false, false);
//   wait(250, msec);
//   shoot(3, 800);  
//   // wait(1, sec);
//   flywheelTarget = 9.2;
//   Move(0, 0, 225, 10, 800, false, false);
  
//   // MoveToPoint(68, 60, 147, 8, 10, 10, 1500); // (68, 60, -213/147)
//   Move(40, 8, 225, 10, 1350, true, false);
//   Move(0, 0, 138, 10, 800, false, false);
//   wait(250, msec);
//   shoot(3, 500);
//   // wait(1, sec);

//   // Get low goal discs
//   flywheelTarget = 9.2;
//   Move(0, 0, 174, 10, 800, false, false);
//   // MoveToPoint(66, 13, 89, 6, 10, 10, 1500); // (66, 13, 89)
//   Move(70, 6, 174, 10, 1480, true, false);
//   // wait(50, msec);
//   Move(0, 0, 75, 10, 800, false, false);

//   // Align to matchload station 2
//   Move(-16, 6, 75, 10, 1000, false, false);
//   Move(0, 0, 89, 10, 800, false, false);
//   wait(250, msec);
//   shoot(10, 600);  // CHANGE TO 10 LATER!!!
//   // wait(1, sec);
//   Move(0, 0, 105, 10, 800, false, false);
//   Move(-47, 9, 105, 10, 1000, false, false);
//   Move(0, 0, 45, 10, 800, false, false);
//   // fire endgame

//   return(0);
// }


// int progSkills() { // States prog: gets three stack instead of low goal
//   resetOdomSensors();
//   intake.spin(forward, 13, volt);
//   flywheelTarget = 9.6;
//   // globalPose.setPoint(31.5, 10.5, 0);

//   // Get first two rollers  
//   Move(-4, 5, 0, 10, 400, false, false);
//   Move(32, 6.7, 321, 9, 1200, true, false);
//   wait(100, msec);
//   Move(0, 0, 88, 10, 900, false, false);
//   wait(100, msec);
//   Move(-8, 8, 88, 10, 700, false, false);
//   wait(150, msec);

//   // Move near mid and shoot 3
//   Move(17, 8, 43, 10, 1000, false, false);
//   wait(100, msec);
//   Move(0, 0, 357, 10, 900, false, false);
//   wait(100, msec);
//   shoot(3, 750);
//   // wait(500, msec);
//   flywheelTarget = 9.4;

//   // Get mid three discs
//   Move(0, 0, 45, 10, 900, false, false); 
//   Move(42, 8, 45, 9, 1700, true, false);
//   wait(50, msec);
//   Move(0, 0, 325, 10, 900, false, false);
//   wait(100, msec);
  
//   intake.spin(reverse, 13, volt);
//   shoot(3, 400);
//   intake.spin(forward, 13, volt);
//   // wait(500, msec);
//   flywheelTarget = 9.2;

//   // Get three from triple stack
//   Move(0, 0, 46, 10, 900, false, false);
//   Move(65.1, 8, 46, 8, 2000, true, false);
//   wait(100, msec);

//   // Get 3rd roller 
//   Move(0, 0, 178, 10, 900, false, false);
//   wait(75, msec);
//   Move(-12, 7, 180, 10, 800, false, false); // -105
//   wait(50, msec);

//   // Line up to matchload station 1
//   Move(5.1, 6, 180, 10, 900, false, false);
//   Move(0, 0, 278, 10, 800, false, false);
//   wait(50, msec);
//   Move(34.5, 6, 278, 8, 1300, true, false); // 53 drive
//   wait(100, msec);
//   Move(0, 0, 252, 7, 1000, false, false);

//   shoot(10, 610); //  CHANGE TO 10 LATER!!!
//   // wait(1000, msec);
//   flywheelTarget = 9.8;
  
//   // Get 4th roller
//   // Move(0, 0, 280, 10, 700, false, false);  // -70
//   // Move(-38, 8, 280, 10, 1200, true, false);  // 285 or 290
//   // wait(150, msec);
//   // Move(0, 0, 180, 10, 800, false, false);
//   Move(0, 0, 320, 10, 800, false, false);
//   Move(-26, 8, 320, 10, 1100, true, false);
//   wait(50, msec);
//   Move(0, 0, 90, 10, 800, false, true);
//   wait(100, msec);
//   Move(39, 5.5, 90, 10, 1200, true, false);
//   wait(50, msec);
//   Move(0, 0, 270, 10, 1000, false, true);
//   wait(50, msec);
//   Move(-16, 6, 270, 10, 900, false, false);

//   // Move to mid and shoot 3
//   Move(16, 6, 220, 10, 1000, true, false);
//   Move(0, 0, 175, 10, 800, false, false);
//   wait(100, msec);
//   intake.spin(reverse, 13, volt);
//   shoot(3, 750);  

//   // wait(1, sec);
//   flywheelTarget = 9.3;
//   Move(0, 0, 225, 10, 800, false, false);
//   intake.spin(forward, 13, volt);

//   Move(44, 8, 225, 10, 1450, true, false);
//   wait(100, msec);
//   Move(0, 0, 138, 10, 800, false, false);
//   wait(100, msec);
//   intake.spin(reverse, 13, volt);
//   shoot(3, 400);
//   intake.spin(forward, 13, volt);
//   // wait(1, sec);

//   // Get low goal discs
//   flywheelTarget = 9.2;
//   Move(0, 0, 174, 10, 800, false, false);
//   Move(50, 7, 174, 10, 1500, true, false);
//   wait(50, msec);
//   Move(0, 0, 70, 10, 800, false, false);

//   // Align to matchload station 2
//   Move(-16, 6, 70, 10, 1000, false, false);
//   Move(0, 0, 89, 10, 800, false, false);
//   wait(100, msec);
//   intake.spin(reverse, 13, volt);
//   shoot(10, 550);  // CHANGE TO 10 LATER!!!
//   // wait(1, sec);
//   Move(0, 0, 105, 10, 800, false, false);
//   Move(-47, 9, 105, 10, 1000, false, false);
//   wait(50, msec);
//   Move(0, 0, 45, 10, 900, false, false);
 
//   // fire endgame
//   toggleAllEndgame();
//   return(0);
// }