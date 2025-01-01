#ifndef Sensor_h
#define Sensor_h
#include "Arduino.h"
#include <SoftwareSerial.h>

class Distance {
private:
  byte _queryDis[8] = {0x50, 0x03, 0x00, 0x34, 0x00, 0x01, 0xC8, 0x45};
  // byte _queryDis[8] = { 0x11, 0x03, 0x00, 0x34, 0x00, 0x01, 0xC7, 0x54 };
  // byte _queryDis[8] = { 0x22, 0x03, 0x00, 0x34, 0x00, 0x01, 0xC2, 0x97 };
  // char send_frame_3[8] = { 0x33, 0x03, 0x00, 0x34, 0x00, 0x01, 0xC1, 0xD6 };
  uint32_t _baud_rate;
  byte receiveData[10];

public:
  Distance(uint32_t baud_rate, int rxPIN, int txPIN);
  int getSensorValue(byte dataForSend[8], uint8_t sizeOfData, uint8_t possion);
  int getDistance();
};
class Soild3in1 {
private:
  byte _queryTemHum[8] = {0x01, 0x03, 0x00, 0x12, 0x00, 0x02, 0x64, 0x0E};
  byte _queryPH[8] = {0x01, 0x03, 0x00, 0x06, 0x00, 0x01, 0x64, 0x0B};
  byte receiveData[10];
  uint32_t _baud_rate;

public:
  Soild3in1(uint32_t _baud_rate, int rxPIN, int txPIN);
  int getSensorValue(byte dataForSend[8], uint8_t sizeOfData, uint8_t possion);
  int getHumidity();    //%
  int getTemperature(); // oC
  int getPH();          // H+
};
class Soild7in1 {
private:
  byte _queryTemHum[8] = {0x01, 0x03, 0x00, 0x12, 0x00, 0x02, 0x64, 0x0E};
  byte _queryN[8] = {0x01, 0x03, 0x00, 0x1E, 0x00, 0x01, 0xE4, 0x0C};
  byte _queryP[8] = {0x01, 0x03, 0x00, 0x1F, 0x00, 0x01, 0xB5, 0xCC};
  byte _queryK[8] = {0x01, 0x03, 0x00, 0x20, 0x00, 0x01, 0x85, 0xC0};
  byte _queryEC[8] = {0x01, 0x03, 0x00, 0x15, 0x00, 0x01, 0x95, 0xCE};
  byte _queryPH[8] = {0x01, 0x03, 0x00, 0x06, 0x00, 0x01, 0x64, 0x0B};
  byte receiveData[9];
  uint32_t _baud_rate;

public:
  Soild7in1(uint32_t baud_rate, int rxPIN, int txPIN);
  int getSensorValue(byte dataForSend[8], uint8_t sizeOfData, uint8_t possion);
  int getTemperature(); // oC
  int getHumidity();    //%
  int getPH();          // H+
  int getNito();        // mg/kg
  int getPhotpho();     // mg/kg
  int getKali();        // mg/kg
  int getEC();          // us/cm
};
class CO2{
private:
  byte _queryCO2[8] = {0x01, 0x03, 0x00, 0x05, 0x00, 0x01, 0x94, 0x0B};
  byte _queryHuATmp[8] = {0x01, 0x03, 0x00, 0x00, 0x00, 0x02, 0xC4, 0x0B};
  uint32_t _baud_rate;
  byte receiveData[10];
public:
  CO2(uint32_t baud_rate, int rxPIN, int txPIN);
  int getSensorValue(byte dataForSend[10], uint8_t sizeOfData, uint8_t possion);
  int getTmp(); // use for get CO2( the unit is oC)
  int getHumidity(); // use for get CO2( the unit is %)
  int getCO2(); // use for get CO2( the unit is ppm)
};
class CH4{
private:
  byte _queryCH4[8] = {0x02, 0x03, 0x00, 0x05, 0x00, 0x01, 0x94, 0x38};
  uint32_t _baud_rate;
  byte receiveData[10];
public:
  CH4(uint32_t baud_rate, int rxPIN, int txPIN);
  int getSensorValue(byte dataForSend[8], uint8_t sizeOfData, uint8_t possion);
  int getCH4(); // use for get CH4( the unit is ppm)
};
#endif