#ifndef communication_h
#define communication_h
#include <WiFiManager.h>
#include <strings_en.h>
#include <wm_consts_en.h>
#include <wm_strings_en.h>
#include <PubSubClient.h>
#include <SPI.h>
#include <Arduino.h>
#include "config.h"
class communication {
private:
  bool res;
public:
  communication();
  void begin();
  void processWiFi();
  bool isWiFiConnnected();
  void connectMqtt();
  void processMQTT();
  void sendToMqtt(String msg, String topic);
};
#endif