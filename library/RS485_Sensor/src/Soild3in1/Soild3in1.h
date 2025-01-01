#ifndef Soild3in1_h
#define Soild3in1_h
#include "Arduino.h"
#include <SoftwareSerial.h>
class Soild3in1 {
private:
  byte _queryTemHum[8] = { 0x01, 0x03, 0x00, 0x12, 0x00, 0x02, 0x64, 0x0E };
  byte _queryPH[8] = { 0x01, 0x03, 0x00, 0x06, 0x00, 0x01, 0x64, 0x0B };
  byte receiveData[10];
  uint32_t _baud_rate;

public:
  Soild3in1(uint32_t _baud_rate, int rxPIN, int txPIN);
  int getSensorValue(byte dataForSend[8], uint8_t sizeOfData, uint8_t possion);
  int getHumidity();     //unit %
  int getTemperature();  //unit oC
  int getPH();           //unit H+
};
#endif