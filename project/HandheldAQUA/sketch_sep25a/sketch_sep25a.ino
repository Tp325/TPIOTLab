#include "Display.h"
Display screen;
Number number;
Pointer pointer;
void covertPage() {
  pointer.buttomFuntion();
  switch (pointer.page) {
    case 0:
      screen.home(pointer);
      pointer.buttomStatusForHome();
      break;
    case 1:
      screen.calibrate(pointer);
      break;
    case 2:
      screen.Auto();
      break;
    case 3:
      // screen.setting();
      break;
    case 4:
      // screen.stored();
      break;
    default:
      Serial.println("ERROR");
  }
}

void setup() {
  screen.begin();
  pointer.begin(12, 2);
  Serial.begin(9600);
}

void loop() {
  covertPage();
}
