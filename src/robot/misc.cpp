#include "robot_header/misc.h"

// Angler toggle
void toggleAngler() { // can simplify to angler.set(!angler.value()) & test later
  bool currentState = angler.value();
  currentState = !currentState;
  angler.set(currentState);
}

// Indexer 
// void indexDisc() {
//   indexer.set(true);
//   wait(250, msec);
//   indexer.set(false);
// }
// bool indexState = indexer.value();

// void shoot(int discNum, int msecDelay) {
//   for(int i = 0; i < discNum; i++) {
//     indexDisc();
//     wait(msecDelay, msec);
//   }
// }

// Endgame toggles
void toggleLeftEndgame() {
  bool currentState = leftEndgame.value();
  currentState = !currentState;
  leftEndgame.set(currentState);
  bottomEndgame.set(currentState);
  flywheelTarget = 0;
}

void toggleRightEndgame() {
  bool currentState = rightEndgame.value();
  currentState = !currentState;
  rightEndgame.set(currentState);
  bottomEndgame.set(currentState);
  flywheelTarget = 0;
}

void toggleBottomEndgame() {
  bool currentState = bottomEndgame.value();
  currentState = !currentState;
  bottomEndgame.set(currentState);
  flywheelTarget = 0;
}

void toggleAllEndgame() {
  bool currentState = leftEndgame.value();
  currentState = !currentState;
  leftEndgame.set(currentState);
  rightEndgame.set(currentState);
  bottomEndgame.set(currentState);
  flywheelTarget = 0;
}