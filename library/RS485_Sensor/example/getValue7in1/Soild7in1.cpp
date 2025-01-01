#include "Soild7in1.h"
#include "Arduino.h"
uint8_t rx = 16;
uint8_t tx = 17;
SoftwareSerial mySerial(rx, tx);
Soild7in1 ::Soild7in1(uint32_t baud_rate, int rxPIN, int txPIN) {
  this->_baud_rate = baud_rate;
  rx = rxPIN;
  tx = txPIN;
}
int Soild7in1::getSensorValue(byte dataForSend[8], uint8_t sizeOfData,
                              uint8_t possion) {
  mySerial.begin(this->_baud_rate);
  delay(200);
  mySerial.write(dataForSend, 8);
  delay(200);
  if (mySerial.available()) {
    mySerial.readBytes(receiveData, sizeOfData - 1);
    return receiveData[possion] << 8 | receiveData[possion + 1];
  } else {
    return 0;
  }
}
int Soild7in1 ::getHumidity() {
  return getSensorValue(_queryTemHum, 10, 3);
}
int Soild7in1 ::getTemperature() {
  return getSensorValue(_queryTemHum, 10, 5);
}
int Soild7in1 ::getPH() {
  return getSensorValue(_queryPH, 8, 3);
}
int Soild7in1 ::getNito() {
  return getSensorValue(_queryN, 8, 3);
}
int Soild7in1 ::getPhotpho() {
  return getSensorValue(_queryP, 8, 3);
}
int Soild7in1 ::getKali() {
  return getSensorValue(_queryK, 8, 3);
}
int Soild7in1 ::getEC() {
  return getSensorValue(_queryEC, 8, 3);
}
