#ifndef __74HC595_h
#define __74HC595_h
#include "Arduino.h"
#include <SPI.h>
class IC74HC595 {
private:
  uint8_t data[48] = { 0 };
  uint8_t DSPin;
  uint8_t STCP;
  uint8_t SHCP;
  uint8_t _MR;
  uint8_t _OE;
  uint8_t sizeOfData;
  uint8_t numberOfIC;
  void setBitToShiftRegister(uint8_t *data);
  void setbit(uint8_t data);

public:
  IC74HC595(uint8_t numberOfIC = 1,
            uint8_t DSPin = 13,
            uint8_t STCP = 21,
            uint8_t SHCP = 12,
            uint8_t _MR = 47,
            uint8_t _OE = 14);
  void begin();
  void setPin(uint8_t IC, uint8_t pin, uint8_t state);
  void setAllHigh();
  void setAllLow();
};

#endif