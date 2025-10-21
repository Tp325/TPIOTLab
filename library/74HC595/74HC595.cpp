#include "74HC595.h"
IC74HC595::IC74HC595(uint8_t numberOfIC,
          uint8_t DSPin,
          uint8_t STCP,
          uint8_t SHCP,
          uint8_t _MR,
          uint8_t _OE) {
  this->numberOfIC = numberOfIC;
  this->DSPin = DSPin;
  this->STCP = STCP;
  this->SHCP = SHCP;
  this->_MR = _MR;
  this->_OE = _OE;
}
void IC74HC595::setbit(uint8_t data) {
  digitalWrite(SHCP, 0);
  SPI.beginTransaction(SPISettings(200000, MSBFIRST, SPI_MODE0));
  SPI.transfer(data);
  SPI.endTransaction();
  digitalWrite(SHCP, 1);
}
void IC74HC595::setBitToShiftRegister(uint8_t *data) {
  digitalWrite(STCP, 0);
  for (int i = 0; i < sizeOfData; i++) {
    setbit(data[i]);
  }
  digitalWrite(STCP, 1);
}
void IC74HC595::begin() {
  sizeOfData = numberOfIC * 8;
  SPI.begin();
  pinMode(_OE, OUTPUT);
  pinMode(_MR, OUTPUT);
  digitalWrite(_OE, 0);
  digitalWrite(_MR, 1);
  setAllLow();
}
void IC74HC595::setPin(uint8_t IC, uint8_t pin, uint8_t state) {
  data[IC * 8 + pin - 1] = state;
  setBitToShiftRegister(data);
}
void IC74HC595::setAllHigh() {
  for (int i = 0; i < sizeOfData; i++) {
    data[i] = 1;
  }
  setBitToShiftRegister(data);
}
void IC74HC595::setAllLow() {
  for (int i = 0; i < sizeOfData; i++) {
    data[i] = 0;
  }
  setBitToShiftRegister(data);
}