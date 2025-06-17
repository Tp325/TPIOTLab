#include "config.h"

//*********************** quản lý hồ****************
float carrierFrequency = 434.0;
float bandwidth = 250.0;
uint8_t spreadingFactor = 7;
uint8_t codingRate = 8;
uint8_t syncWord = 0x34;
uint8_t outputPower = 20;
uint8_t preambleLength = 12;
uint8_t amplifierGain = 0;
//*************communication***********
bool isSended;
String msgToNode;
String msgFromNode;
String msgFromServer;
String msgToServer;

// Thông tin MQTT Broker
bool isWifiConnect = 0;
const char *mqtt_server = "103.221.220.183";
const int mqtt_port = 1883;
const char *mqtt_user = "api1@Iotlab";
const char *mqtt_pass = "Iotlab@2023";
String topicReceive = "TestDemoCTURecieve";
String topicSend = "TestDemoCTUSent";

//************* CircularQueue*****************
 CircularQueue *buffDataFromNode= createQueue(70);
 CircularQueue *buffDataToNode= createQueue(70);
 CircularQueue *buffDataFromServer= createQueue(70);
CircularQueue *buffDataToServer= createQueue(70);