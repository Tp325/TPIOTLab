#include "config.h"

String deviceName = "SOILDDEVICE";
//*****MQTT**********
String msgToMQTT;
const char *mqtt_server = "103.221.220.183";
const int mqtt_port = 1883;
const char *mqtt_user = "api1@Iotlab";
const char *mqtt_pass = "Iotlab@2023";
String topicReceive = "ellReceive";
String topicSend = "HANDHELDDEVICE";

//**********Display****************
int currentPage = 0;
int x = 0, y = 1;
bool isButtonPress = 0;
bool isSellectPress = 0;
int deviceMode = 1;
int isSendPress = 0;

//**************WIFI****************
bool isWifiConnect = 0;
String wifiPass;
String wifiUser;
bool isSaveData = 0;

//*************sensor***************
soilsSensor soilSensor;
float valueOfUartSensor;
String nameOfUartSensor;
