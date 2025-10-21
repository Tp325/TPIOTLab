#ifndef Excution_h
#define Excution_h
#include "Arduino.h"
#include "config.h"
#define Sensor1 32
#define Sensor2 33
class Excution {
private:
  uint8_t switchPin;
  uint8_t ledRedPin;
  uint8_t ledYellowPin;
  uint8_t ledBluePin;
  bool stateRedLed = 0;
  bool stateYellowLed = 0;
  bool stateBlueLed = 0;
  bool isSwitchMode = 1;
public:
  Excution(uint8_t switchPin = 8,
           uint8_t ledRedPin = 6, uint8_t ledYellowPin = 5, uint8_t ledBluePin = 15);
  void begin();
  void setLed(bool stateRedLed, bool stateYellowLed, bool stateBlueLed);
  String checkingMode();
  bool isChangeMode = 0;
};
#endif