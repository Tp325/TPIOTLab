#ifndef CO2_h
#define CO2_h
#include "Arduino.h"
#include <SoftwareSerial.h>
class CO2{
private:
  byte _queryCO2[8] = {0x01, 0x03, 0x00, 0x05, 0x00, 0x01, 0x94, 0x0B};
  uint32_t _baud_rate;
  byte receiveData[10];
public:
  CO2(uint32_t baud_rate, int rxPIN, int txPIN);
  int getSensorValue(byte dataForSend[10], uint8_t sizeOfData, uint8_t possion);
  int getCO2(); // use for get CO2( the unit is ppm)
};
#endif