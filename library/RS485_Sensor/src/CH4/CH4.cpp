#include "CH4.h"
#include "Arduino.h"
uint8_t rx = 16;
uint8_t tx = 17;
CH4::CH4(int rxPIN, int txPIN) {
  rx = rxPIN;
  tx = txPIN;
  mySerial = new SoftwareSerial(rx, tx);
}
CH4::~CH4() {
  delete mySerial;
}
void CH4::begin(long baudrate) {
  mySerial->begin(baudrate);
}
int CH4::getSensorValue(byte dataForSend[8], uint8_t sizeOfData,
                        uint8_t possion) {
  mySerial->write(dataForSend, 8);
  delay(200);
  if (mySerial->available()) {
    mySerial->readBytes(receiveData, sizeOfData - 1);
    return receiveData[possion] << 8 | receiveData[possion + 1];
  } else {
    return 0;
  }
}
int CH4::getCH4() {
  return getSensorValue(_queryCH4, 8, 3);
}