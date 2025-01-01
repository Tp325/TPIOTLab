 #include <LiquidCrystal_I2C.h>
#include "Display.h"
display screen(0x27, 20, 4);
void setup() {
  screen.begin();
  Serial.begin(9600);
}


void loop() {
  screen.screenOn();
}
