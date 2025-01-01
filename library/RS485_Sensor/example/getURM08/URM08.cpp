#include "URM08.h"
URM08::URM08(int RX, int TX) {
  mySerial = new SoftwareSerial(RX, TX);
}
void URM08::begin(long baurate) {
  mySerial->begin(baurate);
}
int URM08::getSensorValue(byte dataForSend[8], uint8_t sizeOfData,
                          uint8_t possion) {

  for (int i = 0; i < 6; i++) {
     mySerial->write(dataForSend[i]);
  }
  delay(200);
  if (mySerial->available()) {
    mySerial->readBytes(receiveData, sizeOfData - 1);
    return receiveData[possion] << 8 | receiveData[possion + 1];
  } else {
    return 0;
  }
}
int URM08::getDistance() {
  return getSensorValue(distanceArray, 8, 5);
}
float URM08::getTemperature() {
  return getSensorValue(temperatureArray, 8, 5) / 10.0;
}
int URM08::setAddress(byte newAddress) {
  byte addressformat[8] = { 0x55, 0xAA, 0xAB, 0x01, 0x55, newAddress, 0x12 };
  for (int i = 0; i <= 7; i++) {
    mySerial->write(addressformat[i]);
  }
  delay(150);
  if (mySerial->available()) {
    mySerial->readBytes(receiveData, 6);
  }
  if (receiveData[5] == 0xCC) {
    return 1;
  } else {
    return 0;
  }
}