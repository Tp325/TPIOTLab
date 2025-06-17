#ifndef Excution_h
#define Excution_h
#include "Arduino.h"
#include "config.h"
#include <SPI.h>
class Execution {
private:
  int numberOfDevice;
  uint8_t digitalValue[8] = { 0x00 };
  void setbit();
  int byteIndex;
  int bitIndex;
public:
  Execution(int numberOfDevice);
  void begin();
  void setAllLow();
  void setAllHigh();
  void setPin(int pin, bool value);
};
#endif