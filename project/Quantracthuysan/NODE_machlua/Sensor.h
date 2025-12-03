#ifndef Sensor_h
#define Sensor_h
#include "Arduino.h"
#include "config.h"
#include <SoftwareSerial.h>
// #include <HardwareSerial>
class Sensor {
private:
  // NH4  - mg/L
  // SAL - PPT
  // PH - H+
  // Tem - oC

  float O2Slope = 4.444;
  float O2Intercept = -0.4444;

  float SALSlope = 18.94;
  float SALIntercept = -3.51;
  // float SALSlope = 16.3;
  // float SALIntercept = 0;
  float PHSlope = -7.78;
  float PHIntercept = 16.34;
  float Vnh4;
  float EmV;
  float Cnh4;
  float slopeTMP = 1;
  float interceptTMP = 0;
  float Ko = 1.02119e-3;
  float K1 = 2.22468e-4;
  float K2 = 1.33342e-7;
  float Rref = 15.0;
  float readVotage(int analogPin);
  int BatSense3V3;
  int BatSense12V;
  HardwareSerial* comunication;
  long baurate;
  float getVoltage(int RawValue);
  int sentToSensor(String data);
  float median(float numbers[], int size);
  float SalinityRaw[5];
  float PHRaw[5];
  float TemRaw[5];
  float NH4Raw[5];
  float O2Raw[5];
public:
  Sensor(int BatSense3V3, int BatSense12V,
         HardwareSerial* comunication, long baurate);
  void begin();
  float readBat();
  float getSalinity();
  float getPH();
  float getNH4();
  float getTemperature();
  float getOxygen();
  void printDebug();
};
#endif