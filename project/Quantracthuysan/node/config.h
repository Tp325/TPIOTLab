#ifndef config_h
#define config_h
#include <Arduino.h>
#include "CircularQueue.h"
#include "Storage.h"
#define timeStartSensor 5000
#define timeToSleep 15ULL * 60ULL * 1000000ULL
#define timeWating 60000
extern NodeManager manager;
struct sensorValue {
  float salinity = 0;
  float pH = 0;
  float nH4 = 0;
  float temperature = 0;
  float pin = 0;
};
extern sensorValue enviromentParameter;
extern uint32_t timeCounter;
#endif