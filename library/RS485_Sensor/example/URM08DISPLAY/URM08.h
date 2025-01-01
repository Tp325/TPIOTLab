#ifndef URM08_h
#define URM08_h
#include "Arduino.h"
#include <SoftwareSerial.h>
class URM08 {
private:
  SoftwareSerial *mySerial;
  byte distanceArray[8] = { 0x55, 0xAA, 0x11, 0x00, 0x02, 0x55+0xAA+0x11+0x00+0x02 };
  byte temperatureArray[8] = { 0x55, 0xAA, 0x11, 0x00, 0x03, 0x13 };
  byte receiveData[10];
  int getSensorValue(byte dataForSend[8], uint8_t sizeOfData, uint8_t possion);
public:
  URM08(int RX, int TX);
  void begin(long baurate);
  int getDistance();             //cm
  float getTemperature();        //oc
  int setAddress(byte address);  // if == 1 => setOK if == 2 => setnotOK
};
#endif