#ifndef Communication_h
#define Communication_h
#include "config.h"
#include <RadioLib.h>
#include <driver/uart.h>
#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
#include <WiFiManager.h>
#include <strings_en.h>
#include <wm_consts_en.h>
#include <wm_strings_en.h>
#include <wm_strings_es.h>
#include <PubSubClient.h>
#include "CircularQueue.h"

#define Nss 21
#define Rst 27
#define Dio0 26
#define Dio1 25

extern DynamicJsonDocument doc;
extern SX1278 radio;
extern WiFiManager wm;
extern WiFiClient espClient;
extern PubSubClient mqttClient;

extern volatile bool receiveFlag;
extern void setReceiveFlag();
extern int state;
extern int trasmitState;
extern bool isSended;

extern bool isWifiConnect;


class Communication {
private:
  nodeInF* Nodebuffer;

  char* mqtt_server;
  int mqtt_port;
  char* mqtt_user;
  char* mqtt_pass;

  float carrierFrequency;
  float bandwidth;
  uint8_t spreadingFactor;
  uint8_t codingRate;
  uint8_t syncWord;
  uint8_t outputPower;
  uint8_t preambleLength;
  uint8_t amplifierGain;

  String clientmqtt;

  String topicReceive;
  String topicSend;

  String msgToNode;
  String msgFromNode;
  String msgFromServer;
  String msgToServer;

  String buffMsgToNode;
  String buffMsgFromNode;
  String buffMsgFromServer;
  String buffMsgToServer;


  bool IDCheckHealth[10] = { 0 };
  bool IDReset[10] = { 0 };
  bool IDGetData[10] = { 0 };

  unsigned long timeOutStartWeb = 0;
  unsigned long timeOutReconnectMQTT = 0;
  unsigned long timeOutReconnectWiFi = 0;
  bool isWebAPStart = 0;

  CircularQueue* buffDataToNode = createQueue(25);
  CircularQueue* buffDataFromNode = createQueue(25);
  CircularQueue* buffDataFromServer = createQueue(25);
  CircularQueue* buffDataToServer = createQueue(25);

  void connectMqttWithTimeOut();
  void callbackmqtt(char* topic, byte* message, unsigned int length);
  static Communication* instance;
  static void callbackWrapper(char* topic, byte* payload, unsigned int length) {
    if (instance != nullptr) {
      instance->callbackmqtt(topic, payload, length);
    }
  }
  void reconnectWifi();
  void reconnectMQTT();
  void processWiFi();
  void processMQTT();
public:
  // Sink (loại = 0)
  // 0 × 256 → 0
  // Khoảng: 0 – 255
  // Node lúa (loại = 1)
  // 1 × 256 → 256
  // Khoảng: 256 – 511
  // Node thủy sản (loại = 2)
  // 2 × 256 → 512
  // Khoảng: 512 – 767
  // Loại kế tiếp (3)
  // Khoảng: 768 – 1023
  Communication(uint16_t SID = 5, String clientmqtt = "Sink005",
                String topicReceive = "Sink005Recieve", String topicSend = "Sink005Sent",
                float carrierFrequency = 434.0, float bandwidth = 250.0, uint8_t spreadingFactor = 7, uint8_t codingRate = 8, uint8_t syncWord = 0x33, uint8_t outputPower = 20, uint8_t preambleLength = 12, uint8_t amplifierGain = 0,
                char* mqtt_server = "dev.iotlab.net.vn", int mqtt_port = 1883, char* mqtt_user = "api1@Iotlab", char* mqtt_pass = "Iotlab@2023");
  void begin();
  void sendToNode();
  void sendToNode(String msg);
  void sendToServer();
  void sendToServer(String msg);
  void receiveFromNode();
  void receiveFromServer();
  void analizeData();
  void process();
  void blocking();
  String mode = "normal";
  uint16_t SID;
  bool haveToReset = 0;
};
#endif