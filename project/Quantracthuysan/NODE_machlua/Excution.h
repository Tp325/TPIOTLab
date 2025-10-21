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
  Excution(int S1 = Sensor1, int S2 = Sensor2,
           uint8_t switchPin = 21,
           uint8_t ledRedPin = 27, uint8_t ledYellowPin = 14, uint8_t ledBluePin = 12);
  void begin();
  void onSensor(int sensor);
  void offSensor(int sensor);
  void setLed(bool stateRedLed, bool stateYellowLed, bool stateBlueLed);
  String checkingMode();
  bool isChangeMode = 0;
};
#endif