#include"CO2.h"
#include "Arduino.h"
uint8_t rx = 16;
uint8_t tx = 17;
SoftwareSerial mySerial(rx, tx);
CO2::CO2(uint32_t _baud_rate, int rxPIN, int txPIN) {
  this->_baud_rate = _baud_rate;
  rx = rxPIN;
  tx = txPIN;
}
int CO2::getSensorValue(byte dataForSend[8], uint8_t sizeOfData,
                             uint8_t possion) {
  mySerial.begin(this->_baud_rate);
  mySerial.write(dataForSend, 8);
  delay(200);
  if (mySerial.available()) {
    mySerial.readBytes(receiveData, sizeOfData - 1);
    return receiveData[possion] << 8 | receiveData[possion + 1];
  } else {
    return 0;
  }
}
int CO2::getCO2() {
  return getSensorValue(_queryCO2, 8, 3);
}