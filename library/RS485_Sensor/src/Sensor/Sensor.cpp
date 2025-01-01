#include "Sensor.h"
#include "Arduino.h"
uint8_t rx = 2;
uint8_t tx = 3;
SoftwareSerial mySerial(rx, tx);
Distance::Distance(uint32_t _baud_rate, int rxPIN, int txPIN) {
  this->_baud_rate = _baud_rate;
  rx = rxPIN;
  tx = txPIN;
}
int Distance::getSensorValue(byte dataForSend[8], uint8_t sizeOfData,
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
int Distance::getDistance() {
  return getSensorValue(_queryDis, 8, 3);
}
Soild3in1::Soild3in1(uint32_t _baud_rate, int rxPIN, int txPIN) {
  this->_baud_rate = _baud_rate;
  rx = rxPIN;
  tx = txPIN;
}
int Soild3in1::getSensorValue(byte dataForSend[8], uint8_t sizeOfData,
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

int Soild3in1 ::getHumidity() {
  return getSensorValue(_queryTemHum, 10, 3);
}
int Soild3in1 ::getTemperature() {
  return getSensorValue(_queryTemHum, 10, 5);
}
int Soild3in1 ::getPH() {
  return getSensorValue(_queryPH, 8, 3);
}
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
int CO2::getTmp() {
  return getSensorValue(_queryHuATmp, 10, 5);

}
int CO2::getHumidity() {
  return getSensorValue(_queryHuATmp, 10, 3);

}
CH4::CH4(uint32_t _baud_rate, int rxPIN, int txPIN) {
  this->_baud_rate = _baud_rate;
  rx = rxPIN;
  tx = txPIN;
}
int CH4::getSensorValue(byte dataForSend[8], uint8_t sizeOfData,
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
int CH4::getCH4() {
  return getSensorValue(_queryCH4, 8, 3);
}