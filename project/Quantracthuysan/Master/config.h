#ifndef config_h
#define config_h
#include <Arduino.h>
#include "CircularQueue.h"

//**************RS485****************
#define RXPin 18
#define TXPin 17

//****************PIN**************************
#define BatSense12V 16
#define BatSense3V3 3

//*************Expanse module*************
extern int SR_LATCH_PIN;
extern int SR_SCK_PIN;
extern int SR_MOSI_PIN;
#define SR_MR_PIN 47
#define SR_OE_PIN 14
#define output_is_enable 0
#define output_is_disable 1
#define register_reset 0
#define register_enable 1

struct Timmer {
  uint8_t day = 0;
  uint8_t date = 0;
  uint8_t month = 0;
  uint8_t year = 0;
  uint8_t hour = 0;
  uint8_t minute = 0;
  uint8_t second = 0;
};
extern Timmer timmer;

//********** Sensor****************
extern int IDOfSensor;

//**************Lorra*****************8
#define SS 10 
#define RST 48
#define DIO0 45
// #define Nss 10
// #define Rst 48
// #define Dio0 45
// #define Dio1 7
extern float carrierFrequency;
extern float bandwidth;
extern uint8_t spreadingFactor;
extern uint8_t codingRate;
extern uint8_t syncWord;
extern uint8_t outputPower;
extern uint8_t preambleLength;
extern uint8_t amplifierGain;

//*************communication***********
extern String msgFromSink;
extern String msgToSink;
extern String buffMsgToSink;
extern String msgToNode;
extern String msgFromNode;
extern String msgFromServer;
extern String msgToServer;
extern bool isSended;

//*******************sensor***************
extern String sensorName;
struct sensor {
  float slope;
  float intercept;
  float value;
};

extern sensor SAL;
extern sensor PH;
extern sensor NH4;
extern sensor TMP;
extern sensor O2;


// Thông tin MQTT Broker
extern bool isWifiConnect;
extern const char *mqtt_server;
extern const int mqtt_port;
extern const char *mqtt_user;
extern const char *mqtt_pass;
extern String topicReceive;
extern String topicSend;

//************* CircularQueue*****************
extern CircularQueue *buffDataFromSink;
extern CircularQueue *buffDataToSink;
extern CircularQueue *buffDataFromNode;
extern CircularQueue *buffDataToNode;
extern CircularQueue *buffDataToServer;
extern CircularQueue *buffDataFromServer;


#endif