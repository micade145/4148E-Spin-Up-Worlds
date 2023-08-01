#pragma once 
#include "vex.h"

// Constants 
extern double SIDE_ENC_OFFSET;
extern double LEFT_ENC_OFFSET;

// Point class
class Point {
  public:
    double x;
    double y;
    double theta;

  void setPoint(double new_x, double new_y, double new_theta) {
    x = new_x;
    y = new_y;
    theta = new_theta;
    Inertial.setRotation(new_theta, deg);
  }
  
  void reset() {
    x = 0.0;
    y = 0.0;
    theta = 0.0;
  }
  // double getX() { return (x); }
  // double getY() { return (y); }
  // double getTheta() { return (theta); }
};

// Global points
extern Point globalPose;
extern Point localPose;
extern Point deltaPose;

// Odometry functions
extern void resetOdomSensors();
extern void updateSensors();
extern void updatePosition();
extern int trackPosition();
