#include "RS485_ES_RAINF_01.h"
ES_RAINF_01::ES_RAINF_01(int RX, int TX) {
  this->RX = RX;
  this->TX = TX;
  mySerial = new SoftwareSerial(this->RX, this->TX);
}
int ES_RAINF_01::getSensorValue(byte dataForSend[8], int address, uint8_t sizeOfData, uint8_t possion) {
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
void ES_RAINF_01::changeAddress(byte dataForSend[8], int oldAddress, int newAddress, uint8_t sizeOfData, uint8_t possion) {
  dataForSend[0] = oldAddress;
  dataForSend[5] = newAddress;
  strcpy(data, dataForSend);
  analize();
  mySerial->write(data, 8);
}
unsigned short ES_RAINF_01::calculateCRC(unsigned char *data, unsigned short length) {
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
void ES_RAINF_01::analize() {
  bufferCrc = calculateCRC(data, 6);
  data[6] = bufferCrc & 0xFF;
  data[7] = (bufferCrc >> 8) & 0xFF;
}
void ES_RAINF_01::begin() {
  mySerial->begin(4800);
}
float ES_RAINF_01::readReadRain(int address) {
  return getSensorValue(bufferReadRain, address, 7, 3) / 10.0;
}
void ES_RAINF_01::modifyAddress(int oldAddress, int newAddress) {
  changeAddress(bufferSetAdress, oldAddress, newAddress, 8, 3);
}
