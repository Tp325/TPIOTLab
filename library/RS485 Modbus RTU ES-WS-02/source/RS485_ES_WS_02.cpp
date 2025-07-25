#include "RS485_ES_WS_02.h"
ES_WS_02::ES_WS_02(int RX, int TX) {
  this->RX = RX;
  this->TX = TX;
  mySerial = new SoftwareSerial(this->RX, this->TX);
}
int ES_WS_02::getSensorValue(byte dataForSend[8], int address, uint8_t sizeOfData, uint8_t possion) {
  dataForSend[0] = address;
  strcpy(data, dataForSend);
  analize();
  mySerial->write(data, 8);
  delay(200);
  if (mySerial->available()) {
    mySerial->readBytes(receiveData, sizeOfData - 1);
    memset(data, 0, sizeof(data));
    return receiveData[possion] << 8 | receiveData[possion + 1];
  } else {
    memset(data, 0, sizeof(data));
    return 404;
  }
}
void ES_WS_02::changeAddress(byte dataForSend[8], int oldAddress, int newAddress, uint8_t sizeOfData, uint8_t possion) {
  dataForSend[0] = oldAddress;
  dataForSend[5] = newAddress;
  strcpy(data, dataForSend);
  analize();
  mySerial->write(data, 8);
}
unsigned short ES_WS_02::calculateCRC(unsigned char *data, unsigned short length) {
  crc = 0xFFFF;
  for (int i = 0; i < length; i++) {
    crc ^= data[i];
    for (int j = 0; j < 8; j++) {
      if (crc & 0x0001) {
        crc = (crc >> 1) ^ 0xA001;
      } else {
        crc >>= 1;
      }
    }
  }
  return crc;
}
void ES_WS_02::analize() {
  bufferCrc = calculateCRC(data, 6);
  data[6] = bufferCrc & 0xFF;
  data[7] = (bufferCrc >> 8) & 0xFF;
}
void ES_WS_02::begin() {
  mySerial->begin(9600);
}
float ES_WS_02::readWindSpeed(int address) {
  return getSensorValue(bufferReadWind, address, 8, 3) / 10.0;
}
void ES_WS_02::modifyAddress(int oldAddress, int newAddress) {
  changeAddress(bufferSetAdress, oldAddress, newAddress, 8, 3);
}
