#ifndef Sensor_h
#define Sensor_h
#include "Arduino.h"
#include "config.h"
#include <SoftwareSerial.h>
class Sensor {
private:
  float slopeTMP = 1;
  float interceptTMP = 0;
  const float Ko = 1.02119e-3;
  const float K1 = 2.22468e-4;
  const float K2 = 1.33342e-7;
  int resistance = 15;
  uint16_t measured_resistance;
  float readVotage(int analogPin);
  int BatSense3V3;
  int BatSense12V;
  SoftwareSerial* comunication;
  long baurate;
  float getVoltage(int RawValue);
  int sentToSensor(String data);
public:
  Sensor(int BatSense3V3, int BatSense12V,
         SoftwareSerial* comunication, long baurate);
  void begin();
  float readBat();
  float getSalinity();
  float getPH();
  float getNH4();
  float getTemperature();
};
#endif