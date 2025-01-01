#ifndef CH4_h
#define CH4_h
#include "Arduino.h"
#include <SoftwareSerial.h>
class CH4 {
private:
  byte _queryCH4[8] = { 0x01, 0x03, 0x07, 0xD0, 0x00, 0x01, 0x84, 0x8d7 };
  byte receiveData[10];
  SoftwareSerial *mySerial;
public:
  CH4(int rxPIN, int txPIN);
  ~CH4();
  int getSensorValue(byte dataForSend[8], uint8_t sizeOfData, uint8_t possion);
  void begin(long baudrate);
  int getCH4();  // use for get CH4( the unit is ppm)
};
#endif