#include "config.h"

// String deviceName = "HANDHELDDEVICE";
String deviceName = "HANDHELDDEVICE1";
//*****MQTT**********
String msgToServer;
String buffmsgToServer;
const char *mqtt_server = "dev.iotlab.net.vn";
const int mqtt_port = 1883;
const char *mqtt_user = "api1@Iotlab";
const char *mqtt_pass = "Iotlab@2023";
// String topicReceive = "HANDHELDDEVICERecieve";
// String topicSend = "HANDHELDDEVICESend";
double LAT;
double LON;
String topicReceive = "HANDHELDDEVICE1Recieve";
String topicSend = "HANDHELDDEVICE1Send";
//**********Display****************
int currentPage = 0;
int x = 0, y = 1;
bool isButtonPress = 0;
bool isSellectPress = 0;
bool isReadyToSleep = 1;
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
//************* CircularQueue*****************
CircularQueue *buffDataToServer = createQueue(20); 
