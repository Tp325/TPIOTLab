#ifndef Sensor_h
#define Sensor_h
#include "Arduino.h"
#include <SoftwareSerial.h>
#include <ModbusMaster.h>
#include <SimpleKalmanFilter.h>
#include <algorithm>

struct SensorCfg {
  uint8_t slaveID;
  uint16_t regAddr;
  uint32_t baud;
  const char *name;
};
class Sensor {
private:
  int getSensorValue(uint8_t slaveID, uint32_t baudrate, uint16_t reg, uint8_t reTries);
  int BatSense3V3;
  SoftwareSerial *comunication;
  ModbusMaster *node;
  SimpleKalmanFilter kalmanTem;
  SimpleKalmanFilter kalmanHum;
  SimpleKalmanFilter kalmanEC;
  SimpleKalmanFilter kalmanPH;
  SimpleKalmanFilter kalmanNi;
  SimpleKalmanFilter kalmanPhotpho;
  SimpleKalmanFilter kalmanKali;
  SimpleKalmanFilter kalmandistance;
  float tempRaw[5];
  float humRaw[5];
  float ECRaw[5];
  float PHRaw[5];
  float niRaw[5];
  float photphoRaw[5];
  float kaliRaw[5];
  float distanceRaw[5];
  float median(float numbers[], int size);
  float readVotage(int RawValue);
public:
  Sensor(int BatSense3V3,
         SoftwareSerial *comunication,
         ModbusMaster *node);
  void begin();
  void getValueOfSensor();
  float getTemperature();  // oC
  float getHumidity();     //%
  float getPH();           // H+
  float getNito();         // mg/kg
  float getPhotpho();      // mg/kg
  float getKali();         // mg/kg
  float getEC();           // us/cm
  float getDistance();     // cm
  float getPinVoltage();   // V
};
#endif