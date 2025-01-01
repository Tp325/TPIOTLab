#ifndef Vernier_h
#define Vernier_h
#include <Wire.h>
#include <math.h>
#include "Arduino.h"

class Vernier {
private:
  byte resistorIDInfo[3][32] = {
    { 67, 97, 114, 98, 111, 110, 32, 68, 105, 111, 120, 105, 100, 101, 67, 79, 50, 32, 32, 32, 32, 32, 32, 32, 112, 112, 109, 32, 32, 32, 32, 0 },   //CO2*
    { 79, 120, 121, 103, 101, 110, 32, 32, 32, 32, 32, 32, 32, 32, 79, 50, 32, 32, 32, 32, 32, 32, 32, 32, 37, 32, 32, 32, 32, 32, 32, 0 },          //O2*
    { 84, 101, 109, 112, 101, 114, 97, 116, 117, 114, 101, 32, 32, 32, 84, 101, 109, 112, 32, 32, 32, 32, 32, 32, 68, 101, 103, 32, 67, 32, 32, 0 }  //Teperature
  };
  const int sensorAddress = 0x50;
  int SIG1Pin;
  int Sig2Pin;
  int VresPin = A4;
  int IDPin = A5;
  int sensorNumber = 0;
  int page;
  float sensorReading;
  byte floatbyte[4];
  void i2cCommunicateToSensor(int sensorAddress, int dataForSend, int possition);
  byte sensorData[128] = {};
  void getInformationOfSensor();
public:
  Vernier(int SIG1Pin, int Sig2Pin);  //A4 VresPin, A5 IdPin
  void autoID();
  // autoID will return all of information bellow
  char shortName[12]={};
  char sensorName[16]={};
  char sensorUnit[7]={};
  float slope=0;
  float intercept=0;
  float readSensor();  //Value of sensor
};
#endif