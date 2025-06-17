#ifndef SensorLib_h
#define SensorLib_h

#include <SoftwareSerial.h>
#include "config.h"
#include "Arduino.h"

class RS485Sensor {
private:
  byte _queryTemHum[8] = { 0x01, 0x03, 0x00, 0x12, 0x00, 0x02, 0x64, 0x0E };
  byte _queryN[8] = { 0x01, 0x03, 0x00, 0x1E, 0x00, 0x01, 0xE4, 0x0C };
  byte _queryP[8] = { 0x01, 0x03, 0x00, 0x1F, 0x00, 0x01, 0xB5, 0xCC };
  byte _queryK[8] = { 0x01, 0x03, 0x00, 0x20, 0x00, 0x01, 0x85, 0xC0 };
  byte _queryEC[8] = { 0x01, 0x03, 0x00, 0x15, 0x00, 0x01, 0x95, 0xCE };
  byte _queryPH[8] = { 0x01, 0x03, 0x00, 0x06, 0x00, 0x01, 0x64, 0x0B };
  byte receiveData[9];
  SoftwareSerial *mySerial;
public:
  RS485Sensor();
  void begin(uint32_t baud_rate);
  int getSensorValue(byte dataForSend[8], uint8_t sizeOfData, uint8_t possion);
  float getTemperature();  //unit oC
  float getHumidity();     //unit %
  float getPH();           //unit H+
  float getNito();         //unit mg/kg
  float getPhotpho();      //unit mg/kg
  float getKali();         //unit mg/kg
  float getEC();           //unit us/cm
};

class AnalogSensor {
private:
  float slopeSAL = 11.6028;
  float interceptSAL = 0;
  float slopePH;
  float interceptPH = 0;
  float slopeNH4;
  float interceptNH4;
  float slopeTMP = 1;
  float interceptTMP = 0;
  const float Ko = 1.02119e-3;
  const float K1 = 2.22468e-4;
  const float K2 = 1.33342e-7;
  int resistance = 15000;
  uint16_t measured_resistance;
  long long int sensorTimmer = 0;
  float RawValue = 0;
  SoftwareSerial *mySerial1;
public:
  AnalogSensor();
  void begin(uint32_t baud_rate);
  void autoScan();
  float getVoltage();
  float getSalinity();     // unit ppt
  float getPH();           // unit H+
  float getNH4();          // unit
  float getTemperature();  // uint oC
};

#endif