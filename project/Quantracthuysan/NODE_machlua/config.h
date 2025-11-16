#ifndef config_h
#define config_h
#include <Arduino.h>
#include "CircularQueue.h"
#include "Storage.h"
#define timeStartSensor 15000
#define timeToSleep 15ULL * 60ULL * 1000000ULL
extern uint32_t timeWating;
extern NodeManager manager;
struct sensorValue {
  float Do;
  float Salinity;
  float PH;
  float NH4;
  float Temperature;
  float pin = 0;
};
extern sensorValue enviromentParameter;
extern uint32_t timeCounter;
#endif