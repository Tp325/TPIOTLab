#ifndef config_h
#define config_h
#include <Arduino.h>
#include "CircularQueue.h"

extern String StationID;

#define RXPin 16
#define TXPin 17

//******************LOra*****************
#define Nss 5
#define Rst 14
#define Dio0 4
#define Dio1 2
#define ledRed 13
extern float carrierFrequency;
extern float bandwidth;
extern uint8_t spreadingFactor;
extern uint8_t codingRate;
extern uint8_t syncWord;
extern uint8_t outputPower;
extern uint8_t preambleLength;
extern uint8_t amplifierGain;

// ************chân nạp code************************
#define CHIP_ENABLE_PIN 4   // Chân kích hoạt Chip Enable trên ESP32
#define RESET_PIN 18        // Chân Reset trên ESP32
#define DATA_COMMAND_PIN 5  // Chân Data/Instruction trên ESP32
#define WRITE_PIN 2         // Chân Write trên ESP32
#define READ_PIN 0          // Chân Read trên ESP32


//*********************** quản lý hồ****************
extern int numberOfPool;
struct Pool {
  byte IDOfSensor;
  float SensorpieLenght;
  unsigned long int timeReposeDataSensorToSink = 0;
  float mucnuoc;
  float maxValue;
  float midValue;
  float minValue;
  int poolID;
  int supplyPin;
  bool inStatus;
  int drainPin;
  bool outStatus;
  int isSentValStatus = 0;
  int stepOfAuto = 0;
  bool autoStatus;
  bool isDoneAutoMode = 0;
  bool isSentSensorStatus = 0;
};
extern Pool pool[10];
extern void createNewPool(int poolID, int supplyPin, int drainPin, byte IDOfSensor, float SensorpieLenght);


//*************communication***********
extern String msgFromSink;
extern String msgToSink;
extern String buffMsgFromSink;
extern bool isSended;
extern bool haveToReset;

//************* CircularQueue*****************
extern CircularQueue *buffDataFromSink;
extern CircularQueue *buffDataToSink;


//***************Execution*******************
extern int IDOfPool;
extern int VALVE_SUPPLY_1;
extern int VALVE_DRAIN_1;
extern int VALVE_SUPPLY_2;
extern int VALVE_DRAIN_2;
extern int VALVE_SUPPLY_3;
extern int VALVE_DRAIN_3;


#endif