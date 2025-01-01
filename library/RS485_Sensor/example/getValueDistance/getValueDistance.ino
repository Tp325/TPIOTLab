#include "Distance.h"
//RX pin 16 ESP32
//TX pin 17 ESP32
Distance distance(115200,16,17);
void setup() {
  Serial.begin(9600);
}
void loop() {
Serial.println(distance.getDistance());
  delay(1000);
}
