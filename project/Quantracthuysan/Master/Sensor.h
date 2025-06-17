#ifndef Sensor_h
#define Sensor_h
#include "Arduino.h"
#include "config.h"
class Sensor {
private:
  float slopeTMP = 1;
  float interceptTMP = 0;
  const float Ko = 1.02119e-3;
  const float K1 = 2.22468e-4;
  const float K2 = 1.33342e-7;
  int resistance = 15;
  uint16_t measured_resistance;
  float readVotage(int analogPin);
public:
  Sensor();
  void begin();
  float getVoltage(int RawValue);
  float readBat(int analogPin);
  float getSalinity(int RawValue);
  float getPH(int RawValue);
  float getNH4(int RawValue);
  float getTemperature(int RawValue);
};
#endif