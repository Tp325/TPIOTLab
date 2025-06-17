#ifndef Sensor_h
#define Sensor_h
#include "Arduino.h"
#include "config.h"
class Sensor {
private:
public:
  Sensor();
  void begin();
  float readVotage(int analogPin);
};
#endif