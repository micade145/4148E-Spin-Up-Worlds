#pragma once
#include "vex.h"

class PID {
private: 
  double kP = 0;
  double kD = 0;
  
  double error = 0;
  double lastError = 0;
  double derivative = 0;
  double output = 0;
  
public:
  PID(double m_kP, double m_kD) {
    kP = m_kP;
    kD = m_kD;
  }
  double calculateOutput(double m_error);
  double calculate(double pidTarget, double pidCurrent);
  double getError();
  void reset();
};
