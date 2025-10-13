#include "74HC595.h"
IC74HC595::IC74HC595(uint8_t numberOfIC, uint8_t STCP, uint8_t _MR, uint8_t _OE) {
  this->numberOfIC = numberOfIC;
  this->STCP = STCP;
  this->_MR = _MR;
  this->_OE = _OE;
  data = new uint8_t[numberOfIC];   
}
void IC74HC595::begin() {
  SPI.begin();
  pinMode(STCP, OUTPUT);
  pinMode(_MR, OUTPUT);
  pinMode(_OE, OUTPUT);
  digitalWrite(_OE, LOW);   
  digitalWrite(_MR, HIGH);  
  setAllLow();
}
void IC74HC595::sendData() {
  digitalWrite(STCP, LOW);
  for (int i = numberOfIC - 1; i >= 0; i--) {
    SPI.transfer(data[i]);
  }
  digitalWrite(STCP, HIGH);
}
void IC74HC595::setPin(uint8_t IC, uint8_t pin, uint8_t state) {
  if (IC >= numberOfIC || pin > 7) return;
  bitWrite(data[IC], pin, state);
  sendData();
}
void IC74HC595::setAllHigh() {
  for (int i = 0; i < numberOfIC; i++) {
    data[i] = 0xFF;
  }
  sendData();
}
void IC74HC595::setAllLow() {
  for (int i = 0; i < numberOfIC; i++) {
    data[i] = 0x00;
  }
  sendData();
}
