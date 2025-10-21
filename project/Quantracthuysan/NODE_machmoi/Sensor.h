#ifndef Sensor_h
#define Sensor_h
#include "Arduino.h"
#include "config.h"
#include <SoftwareSerial.h>
class Sensor {
private:
  float SALSlope;
  float SALIntercept;
  float PHSlope = -7.78;
  float PHIntercept = 16.34;
  float Vnh4;  // đọc điện áp cảm biến (0–5V)
  float EmV;   // chuyển sang mV theo công thức Vernier
  float Cnh4;  // tính nồng độ mg/L
  float slopeTMP = 1;
  float interceptTMP = 0;
  float Ko = 1.02119e-3;
  float K1 = 2.22468e-4;
  float K2 = 1.33342e-7;
  float Rref = 15.0;
  float readVotage(int analogPin);
  int BatSense3V3;
  int BatSense12V;
  SoftwareSerial* comunication;
  long baurate;
  float getVoltage(int RawValue);
  int sentToSensor(String data);
  float SalinityRaw[5];
  float PHRaw[5];
  float TemRaw[5];
  float NH4Raw[5];
  float median(float numbers[], int size);
public:
  Sensor(int BatSense3V3, int BatSense12V,
         SoftwareSerial* comunication, long baurate);
  void begin();
  float readBat();
  float getSalinity();
  float getPH();
  float getNH4();
  float getTemperature();
  void getValueOfSensor();
};
#endif