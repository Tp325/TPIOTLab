#ifndef RS485ForAIR_H
#define RS485ForAIR_H
#include "Arduino.h"
#include <SoftwareSerial.h>
class RS485ForAir{
private:

public:

};
class CO2{
private:
  byte _queryCO2[8] = {0x01, 0x03, 0x00, 0x05, 0x00, 0x01, 0x94, 0x0B};
  uint32_t _baud_rate;
  byte receiveData[10];
public:
  CO2(uint32_t baud_rate,SoftwareSerial *mySerial);
  int getSensorValue(byte dataForSend[10], uint8_t sizeOfData, uint8_t possion);
  int getCO2(); // use for get CO2( the unit is ppm)
  friend class RS485ForAir;
};

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
  friend class RS485ForAir;
};



#endif