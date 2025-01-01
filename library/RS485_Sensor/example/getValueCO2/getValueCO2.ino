#include"CO2.h"
CO2 co2(9600,16,17);
//RX pin 16 ESP32
//TX pin 17 ESP32
void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.println("CO2: ");
  Serial.println(co2.getCO2());
  delay(1000);
}
