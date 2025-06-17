#ifndef communication_h
#define communication_h
#include <WiFiManager.h>
#include <strings_en.h>
#include <wm_consts_en.h>
#include <wm_strings_en.h>
#include <PubSubClient.h>
#include "CircularQueue.h"
#include <SPI.h>
#include <Arduino.h>
#include "config.h"
class communication {
private:
  void connectMqttWithTimeOut();
  bool res;
  unsigned long timeOutReconnectMQTT = 0;
  unsigned long timeOutReconnectWiFi = 0;
  bool isWebAPStart = 0;
public:
  communication();
  void begin();
  void processWiFi();
  void reconnectWifi();
  bool isWiFiConnnected();
  void reconnectMQTT();
  void processMQTT();
  void sendToServer(String msg);
  void sendToServer();
};
#endif