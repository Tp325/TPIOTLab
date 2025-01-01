#ifndef Distance_h
#define Distance_h
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
  int getDistance(); // use for get Distance of Sensor( the unit is mm)
};

#endif