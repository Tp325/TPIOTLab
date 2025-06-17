#ifndef config_h
#define config_h
#include <Arduino.h>
#include "CircularQueue.h"

extern void setID(int ID);

//**************RS485****************
#define RXPin 10
#define TXPin 11


//********** Sensor****************
#define sig1Pin A1
extern int IDOfSensor;
extern String command;


//*************communication***********
extern bool sendSensorData;
extern String msgFromMaster;
extern String msgToMaster;

//************* CircularQueue*****************
extern CircularQueue *buffDataFromMaster;
extern CircularQueue *buffDataToMaster;

#endif