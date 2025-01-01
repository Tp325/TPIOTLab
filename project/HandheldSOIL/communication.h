#ifndef communication_h
#define communication_h
#include <WiFiManager.h>
#include <strings_en.h>
#include <wm_consts_en.h>
#include <wm_strings_en.h>
#include <PubSubClient.h>
#include <SPI.h>
#include <Arduino.h>
class communication {
private:
  bool res;
  // Thông tin MQTT Broker
  const char* mqtt_server = "broker.hivemq.com";
  const int mqtt_port = 1883;
  WiFiManager* wm;
public:
  communication();
  void begin();
  void connectToWiFi();
  bool isWiFiConnnected();
  void connectMqtt();
  void sendToMqtt(String msg, String topic);
};
#endif