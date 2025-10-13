#ifndef __74HC595_h
#define __74HC595_h
#include "Arduino.h"
#include <SPI.h>
#pragma once
class IC74HC595 {
private:
  uint8_t numberOfIC;  
  uint8_t STCP;        
  uint8_t _MR;        
  uint8_t _OE;    
  uint8_t *data;      
  void sendData();
public:
  IC74HC595(uint8_t numberOfIC = 2, uint8_t STCP = 21, uint8_t _MR = 47, uint8_t _OE = 14);
  void begin();
  void setPin(uint8_t IC, uint8_t pin, uint8_t state);
  void setAllHigh();
  void setAllLow();
};

#endif