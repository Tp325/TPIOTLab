#ifndef RS485_ES_WS_04_h
#define RS485_ES_WS_04_h
#include <Arduino.h>
#include <SoftwareSerial.h>
class ES_WS_04
{
private:
    int RX;
    int TX;
    byte data[8];
    byte receiveData[10];
    byte bufferWindDirection[8] = {0x01, 0x03, 0x00, 0x00, 0x00, 0x01, 0x84, 0x0A};
    String dataReceive;
    unsigned short crc;
    unsigned short bufferCrc;
    unsigned char crcHigh;
    unsigned char crcLow;
    unsigned short calculateCRC(unsigned char *data, unsigned short length);
    void analize();
    int getSensorValue(byte dataForSend[8], int address, uint8_t sizeOfData, uint8_t possion);
    SoftwareSerial *mySerial;
public:
    ES_WS_04(int RX, int TX);
    void begin();
    float readWindDirection(int address);
    /***********
     *  0 gió bắc
     * 45 gió đông bắc
     * 90 gió đông
     * 135 gió đông nam
     * 190 gió nam
     * 225 gió tây nam 
    **************/
};
#endif