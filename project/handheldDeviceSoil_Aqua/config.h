#ifndef Config_h
#define Config_h
#include "Arduino.h"
extern String deviceName;

//***********MQTT**************
extern String msgToMQTT;
extern const char *mqtt_server;
extern const int mqtt_port;
extern const char *mqtt_user;
extern const char *mqtt_pass;


//**********Display****************
extern int currentPage;
extern int x, y;
extern bool isButtonPress;
extern bool isSellectPress;
extern int deviceMode;
extern int isSendPress;

//**************WIFI****************
extern bool isWifiConnect;
extern String wifiPass;
extern String wifiUser;
extern bool isSaveData;

//*************sensor***************
struct soilsSensor {
  float tem;
  float humidity;
  float EC;
  float PH;
  float nito;
  float photpho;
  float kali;
};
extern soilsSensor soilSensor;
extern float valueOfUartSensor;
extern String nameOfUartSensor;


#endif