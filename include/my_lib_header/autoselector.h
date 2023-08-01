#pragma once
#include "vex.h"
#include <string> 

// Variables
bool enableSelect = false;
int autonToRun = 0;

class Button {
  public:
    int x, y, width, height;
    std::string text;
    std::string autoName;
    vex::color buttonColor, textColor;

    Button(int x, int y, int width, int height, std::string text, vex::color buttonColor, vex::color textColor, std::string autoName)
    : x(x), y(y), width(width), height(height), text(text), buttonColor(buttonColor), textColor(textColor), autoName(autoName){};

    void render() {
      Brain.Screen.drawRectangle(x, y, width, height, buttonColor);
      wait(5, msec);
      // Brain.Screen.printAt(x + 30, y + 30, false, text.c_str());
      Brain.Screen.printAt(x + width / 3, y + height / 2, false, text.c_str());
    }
    
    bool isClicked() {
      if(Brain.Screen.pressing() && Brain.Screen.xPosition() >= x && Brain.Screen.xPosition() <= x + width &&
      Brain.Screen.yPosition() >= y && Brain.Screen.yPosition() <= y + height) { return true; }
      else {return false;}
    }
};

// Alliance Buttons
Button redAlliance = Button(390, 20, 60, 70, "RED", vex::black, vex::white, "");
Button blueAlliance = Button(390, 110, 60, 70, "BLUE", vex::black, vex::white, "");

// Auto buttons
Button autonButtons[] = {
  Button(20, 20, 70, 70, "1", vex::white, vex::black, "Left Side WP"),
  Button(120, 20, 70, 70, "2", vex::white, vex::black, "Right Side WP"),
  Button(220, 20, 70, 70, "3", vex::white, vex::black, "Devious Right Side WP"),
  Button(20, 110, 70, 70, "4", vex::white, vex::black, "Solo WP"),
  Button(120, 110, 70, 70, "5", vex::white, vex::black, "mystery??"),
  Button(220, 110, 70, 70, "6", vex::white, vex::black, "Prog Skills (please work)")
};

// rgb stuff that doesn't work
int rVal = 255;
int gVal = 0;
int bVal = 0;

int wTime = 250;
int cycleRGB() {
  while(true) {
    for(int i=0; i<255; i++){
      rVal -= 1;
      gVal += 1;
      wait(wTime, msec);
    }
    for(int j=0; j<255; j++){
      gVal -=1;
      bVal +=1;
      wait(wTime, msec);
    }
    for(int k=0; k<255; k++){
      bVal -=1;
      rVal +=1;
      wait(wTime, msec);
    }
  }
}