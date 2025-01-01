#include "URM08.h"
URM08 sensor(2, 3);
void setup() {
  sensor.begin(19200);
  Serial.begin(9600);
}

void loop() {
  delay(10000);
  Serial.print("Distance:  ");
  Serial.println(sensor.getDistance());
  delay(500);
   Serial.print("Temperature:  ");
  Serial.println(sensor.getTemperature());
  delay(500);
}
