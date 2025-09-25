#ifndef Communication_h
#define Communication_h
#include "config.h"
#include <RadioLib.h>
#include <ArduinoJson.h>
#include <ArduinoJson.hpp>

#define Nss 5
#define Rst 13
#define Dio0 2
#define Dio1 22

extern DynamicJsonDocument doc;
extern SX1278 radio;

extern volatile bool receiveFlag;
extern void setReceiveFlag();
extern int state;
extern int trasmitState;
extern bool isSended;

class Communication {
private:
  uint32_t receiveTimeOut = 0;
  uint32_t timeOutACK = 0;
  bool isReceiveACK = 1;
  bool isWaitingACK = 0;
  uint8_t tryToSend = 0;


  float carrierFrequency;
  float bandwidth;
  uint8_t spreadingFactor;
  uint8_t codingRate;
  uint8_t syncWord;
  uint8_t outputPower;
  uint8_t preambleLength;
  uint8_t amplifierGain;

  String msgToSink;
  String buffMsgToSink;
  String msgFromSink;
  String buffMsgFromSink;

  CircularQueue* buffDataToSink = createQueue(50);
  CircularQueue* buffDataFromSink = createQueue(50);

  bool checkingACK(DynamicJsonDocument document);
public:
  uint16_t stationID;
  uint16_t sensorID;
  Communication(uint16_t sensorID = 261,
                float carrierFrequency = 434.0, float bandwidth = 250.0, uint8_t spreadingFactor = 7, uint8_t codingRate = 8, uint8_t syncWord = 0x33, uint8_t outputPower = 20, uint8_t preambleLength = 12, uint8_t amplifierGain = 0);
  void begin();
  void sendToSinkWithACK();
  void sendToSink(String msg);
  void receiveFromSink();
  void installationMode();
  bool hasMsgFromSink();
  bool hasMsgToSink();
  bool scanNewSink();
  void analize(sensorValue enviromentParameter);
  float RSSI = -404;
  float SNR = -404;
  bool haveToReset = 0;
  bool isScanningNewSink = 0;
};
#endif