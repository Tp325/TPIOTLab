#ifndef RS485_ES_WS_02_h
#define RS485_ES_WS_02_h
#include <Arduino.h>
#include <SoftwareSerial.h>
class ES_WS_02
{
private:
    int RX;
    int TX;
    byte data[8];
    byte receiveData[10];
    byte bufferReadWind[8] = {0x01, 0x03, 0x04, 0x00, 0x24, 0x03, 0xFA, 0x39};
    byte bufferSetAdress[8] = {0x01, 0x06, 0x01, 0x00, 0x00, 0x02, 0x09, 0xF7};
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
    ES_WS_02(int RX, int TX);
    void begin();
    float readWindSpeed(int address);
    void modifyAddress(int oldAddress, int newAddress);
};
#endif