#include "Sensor.h"

Sensor::Sensor() {
}
void Sensor::begin() {
  pinMode(sig1Pin,INPUT);
}
float Sensor::readVotage(int analogPin) {
  return analogRead(analogPin) * (5 / 1023);
}
