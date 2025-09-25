#ifndef config_h
#define config_h
#include <Arduino.h>
#include "CircularQueue.h"
#include "Storage.h"
#define timeStartSensor 5000
#define timeToSleep 15ULL * 60ULL * 1000000ULL 
extern uint32_t timeWating ;
extern NodeManager manager;
struct sensorValue {
  float Tem = 0;
  float Hum = 0;
  float EC = 0;
  float PH = 0;
  float Ni = 0;
  float Photpho = 0;
  float Kali = 0;
  float distance = 0;
  float pin = 0;
};
extern sensorValue enviromentParameter;
extern uint32_t timeCounter;
#endif