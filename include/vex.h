/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       vex.h                                                     */
/*    Author:       Vex Robotics                                              */
/*    Created:      1 Feb 2019                                                */
/*    Description:  Default header for V5 projects                            */
/*                                                                            */
/*----------------------------------------------------------------------------*/
#pragma once
#include <math.h>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "v5.h"
#include "v5_vcs.h"

#include "robot-config.h"
// Robot header
#include "robot_header/drive.h"
#include "robot_header/flywheel.h"
#include "robot_header/intake.h"
#include "robot_header/misc.h"
#include "robot_header/vision.h"

#include "globals.h"
#include "autos.h"

// wow its our ibrary 
#include "my_lib_header/pid.h"
#include "my_lib_header/odom.h"
#include "my_lib_header/motion.h"
// #include "my_lib_header/autoselector.h"

#define waitUntil(condition)                                                   \
  do {                                                                         \
    wait(5, msec);                                                             \
  } while (!(condition))

#define repeat(iterations)                                                     \
  for (int iterator = 0; iterator < iterations; iterator++)