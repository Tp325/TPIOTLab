#include"CH4.h"
CH4 ch4(2,3);
//RX pin 16 ESP32
//TX pin 17 ESP32
void setup() {
  Serial.begin(9600);
  ch4.begin(4800);
}

void loop() {
  Serial.println("CH4: ");
  Serial.println(ch4.getCH4());
  delay(1000);
}
