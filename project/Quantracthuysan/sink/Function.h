#ifndef Function_h
#define Function_h
#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
#include <SoftwareSerial.h>
#include "Arduino.h"
#include <WiFi.h>
#include <PubSubClient.h>
#include "esp_sleep.h"
class Rs485Sensor {
private:
  const float Ko = 1.02119e-3;
  const float K1 = 2.22468e-4;
  const float K2 = 1.33342e-7;
  uint16_t resistance = 17;
  float _slope_temp = 1.0915;
  float _intercept_temp = -3.3994;
  // sal
  float slope1 = -7.78;
  float intercept1 = 16.34;
  // PH
  float slope = 16.3;
  float intercept = 0.0;
  //NH4
  float _slope_NH4 = 1;
  float _intercept_NH4 = 0;
  // do
  float _slope_DO = 4.444;
  float _intercept_DO = -0.4444;
public:
  Rs485Sensor();
  float getDo(int AnalogInput);
  float getNH4(int AnalogInput);
  float getPh(int AnalogInput);
  float getTmp(int AnalogInput);
  float getSal(int AnalogInput);
  float getvoltage(int AnalogInput);
};

class TransmitToNode {
private:
public:
  TransmitToNode();
  void start();
  void send(int Id);
  void fillterData(Rs485Sensor Sensor);
  void receive(String DataReceiveFromNode, Rs485Sensor Sensor);
  void processDataFromNode(Rs485Sensor Sensor);
  String processedData;
  float arrayProcessedData[6];
  /*
  use arrayProcessedData after function processDataFromNode (it is an array) with:
  _ slot 1 is getSal
  _ slot 2 is Tmp
  _ slot 3 is Ph
  _ slot 4 is NH4
  _ slot 5 is Do
  */
};

class Wifi {
private:
public:
  Wifi();
  void start(char* userName, char* password);
};

class TransmitToSever {
private:
  const char* mqttsever = "mqtt.cusc.vn";
  const int mqttport = 1883;
  const char* mqttuser = "Hardware_Test";
  const char* mqttpass = "test@123";
public:
  TransmitToSever();
  void start();
  void reconnect();
  void sendDataToSever(String dataForSend, char* topic);
};

class Sleep {
private:
public:
  Sleep();
  void setup(float minuteToSleep);
  void begin();
};
#endif