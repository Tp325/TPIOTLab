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
  float windSpeed = 0;
  String windDirection;
  float airTemperature = 0;
  float airHumidity = 0;
  float airPressure = 0;
  float vaporPressureDeficit = 0;
  float rain = 0;
  float waterTem = 0;
  float pin = 0;
};
extern sensorValue enviromentParameter;
extern uint32_t timeCounter;
#endif