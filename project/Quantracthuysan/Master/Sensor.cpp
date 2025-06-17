#include "Sensor.h"

Sensor::Sensor() {
}
void Sensor::begin() {
  pinMode(BatSense3V3, INPUT);
  pinMode(BatSense12V, INPUT);
}
float Sensor::readVotage(int analogPin) {
  return analogRead(analogPin) * (3.3 / 4095.0);
}

float Sensor::readBat(int analogPin) {
  return readVotage(analogPin) * 1.68;
}
float Sensor ::getVoltage(int RawValue) {
  return (RawValue * 5.0) / 1023.0;
}
float Sensor ::getSalinity(int RawValue) {
  return (getVoltage(RawValue) * SAL.slope + SAL.intercept);
}
float Sensor ::getPH(int RawValue) {
  return (getVoltage(RawValue) * PH.slope + PH.intercept);
}
float Sensor ::getNH4(int RawValue) {
  return ((getVoltage(RawValue) * NH4.slope + NH4.intercept) > 0 ? (getVoltage(RawValue) * NH4.slope + NH4.intercept) : 0.0);
}
float Sensor ::getTemperature(int RawValue) {
  measured_resistance = (resistance * getVoltage(RawValue)) / (5.0 - getVoltage(RawValue));
  return (1.0 / (Ko + K1 * logf(1000.0 * measured_resistance) + K2 * powf(logf(1000.0 * measured_resistance), 3)) - 273.15);
}
