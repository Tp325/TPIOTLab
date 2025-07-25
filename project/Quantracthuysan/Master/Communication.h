#ifndef Communication_h
#define Communication_h
#include "config.h"
// #include <RadioLib.h>
#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
#include <SoftwareSerial.h>
#include <driver/uart.h>
#include <SPI.h>
#include <LoRa.h>

// #include <WiFiManager.h>
// #include <strings_en.h>
// #include <wm_consts_en.h>
// #include <wm_strings_en.h>
// #include <wm_strings_es.h>
// #include <PubSubClient.h>
extern DynamicJsonDocument doc;
// extern SX1278 radio;
// extern volatile bool receiveFlag;
// extern void setReceiveFlag();
// extern int state;
// extern int trasmitState;
class Communication {
private:
  void connectMqtt();
  void callbackmqtt(char* topic, byte* message, unsigned int length);
  static Communication* instance;
  static void callbackWrapper(char* topic, byte* payload, unsigned int length) {
    instance->callbackmqtt(topic, payload, length);
  }
public:
  Communication();
  void begin();
  void receiveFromSink();
  bool receiveFromNode();
  void sendToNode(String msg);
  void sendToNode();
  void sendToSink();
  void sendToSink(String msg);
  void receiveFromServer();
  void sendToServer();
  void processWiFi();
  void processMQTT();
};
#endif