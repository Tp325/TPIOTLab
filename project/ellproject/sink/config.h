#ifndef config_h
#define config_h
#include <Arduino.h>
#include "CircularQueue.h"

extern String StationID;

//******************LOra*****************
#define Nss 21
#define Rst 27
#define Dio0 26
#define Dio1 25
#define ledRed 13
extern float carrierFrequency;
extern float bandwidth;
extern uint8_t spreadingFactor;
extern uint8_t codingRate;
extern uint8_t syncWord;  // tùy chỉnh đường mạng
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
  int poolID;
  float mucnuoc;
  bool inStatus;
  bool outStatus;
  bool autoStatus;
  float maxValue;
  float midValue;
  float minValue;
};
extern Pool pool[10];

//*************communication***********
extern String msgFromDisplay;
extern String msgToDisplay;
extern String msgToNode;
extern String msgFromNode;
extern String msgFromServer;
extern String msgToServer;
extern bool isSended;
extern bool haveToReset;

//************* CircularQueue*****************
extern CircularQueue *buffDataFromDisplay;
extern CircularQueue *buffDataFromNode;
extern CircularQueue *buffDataFromServer;
extern CircularQueue *buffDataToServer;


// Thông tin MQTT Broker
extern bool isWifiConnect;
extern const char *mqtt_server;
extern const int mqtt_port;
extern const char *mqtt_user;
extern const char *mqtt_pass;
extern String topicReceive;
extern String topicSend;
#endif