#include "Vernier.h"
Vernier vernier(A0, A1);
void setup() {
  Serial.begin(9600);
  vernier.autoID();
  Serial.println(vernier.sensorName);
  Serial.println(vernier.shortName);
  Serial.println(vernier.sensorUnit);
  Serial.println(vernier.intercept);
  Serial.println(vernier.slope);
}

void loop() {
  Serial.println(vernier.readSensor());
}
