#ifndef Soild7in1_h
#define Soild7in1_h
#include "Arduino.h"
#include <SoftwareSerial.h>
class Soild7in1 {
private:
  byte _queryTemHum[8] = { 0x01, 0x03, 0x00, 0x12, 0x00, 0x02, 0x64, 0x0E };
  byte _queryN[8] = { 0x01, 0x03, 0x00, 0x1E, 0x00, 0x01, 0xE4, 0x0C };
  byte _queryP[8] = { 0x01, 0x03, 0x00, 0x1F, 0x00, 0x01, 0xB5, 0xCC };
  byte _queryK[8] = { 0x01, 0x03, 0x00, 0x20, 0x00, 0x01, 0x85, 0xC0 };
  byte _queryEC[8] = { 0x01, 0x03, 0x00, 0x15, 0x00, 0x01, 0x95, 0xCE };
  byte _queryPH[8] = { 0x01, 0x03, 0x00, 0x06, 0x00, 0x01, 0x64, 0x0B };
  byte receiveData[9];
  uint32_t _baud_rate;

public:
  Soild7in1(uint32_t baud_rate, int rxPIN, int txPIN);
  int getSensorValue(byte dataForSend[8], uint8_t sizeOfData, uint8_t possion);
  int getTemperature();  // oC
  int getHumidity();     //%
  int getPH();           // H+
  int getNito();         // mg/kg
  int getPhotpho();      // mg/kg
  int getKali();         // mg/kg
  int getEC();  // us/cm
};
#endif