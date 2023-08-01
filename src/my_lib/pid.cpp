#include "my_lib_header/pid.h"

// Credit to Theo | 7842F on VEXForum


double PID::calculateOutput(double m_error) {
  error = m_error;
  
  // //calculate delta time
  // double dT = m_timer.millis().convert(millisecond) - m_lastTime;
  // //abort if dt is too small
  // if(dT < m_minDt) return m+output;

  //calculate derivative
  derivative = (error - lastError);

  //calculate output
  output = (error * kP) + (derivative * kD);

  //save values
  // m_lastTime = m_timer.millis().convert(millisecond);
  lastError = error;
  
  return output;
}

double PID::calculate(double pidTarget, double pidCurrent) {
  return calculateOutput(pidTarget - pidCurrent);
}

double PID::getError() {
  return error;
}

void PID::reset() {
  error = 0;
  lastError = 0;
  derivative = 0;
  output = 0;
}

