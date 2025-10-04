#ifndef RS485_ES_RAINF_01_h
#define RS485_ES_RAINF_01_h
#include <Arduino.h>
#include <SoftwareSerial.h>
class ES_RAINF_01
{
private:
    int RX;
    int TX;
    byte data[8];
    byte receiveData[10];
    byte bufferReadRain[8] = {0x01, 0x03, 0x00, 0x00, 0x00, 0x01, 0x84, 0x0A};
    byte bufferSetAdress[8] = {0x01, 0x03, 0x07, 0xD0, 0x00, 0x01, 0x84, 0x87};
    String dataReceive;
    unsigned short crc;
    unsigned short bufferCrc;
    unsigned char crcHigh;
    unsigned char crcLow;
    unsigned short calculateCRC(unsigned char *data, unsigned short length);
    void analize();
    int getSensorValue(byte dataForSend[8], int address, uint8_t sizeOfData, uint8_t possion);
    void changeAddress(byte dataForSend[8], int oldAddress, int newAddress, uint8_t sizeOfData, uint8_t possion);
    SoftwareSerial *mySerial;
public:
    ES_RAINF_01(int RX, int TX);
    void begin();
    float readReadRain(int address);
    void modifyAddress(int oldAddress, int newAddress);
};
#endif