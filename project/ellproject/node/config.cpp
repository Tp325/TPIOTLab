#include "config.h"

// String StationID = "TESTH6CT";
// String StationID = "TramluonNgaNam1";
// String StationID = "TramluonNgaNam2";

// String StationID = "TramluonNgaNam3";
String StationID = "TramluonNgaNam4";
// String StationID = "TramluonNgaNam5";
// String StationID = "TramluonNgaNam6";



//*********************** quản lý hồ****************
Pool pool[10];
int numberOfPool = 0;
void createNewPool(int poolID, int supplyPin, int drainPin, byte IDOfSensor, float SensorpieLenght) {
  pool[poolID].poolID = poolID;
  pool[poolID].supplyPin = supplyPin;
  pool[poolID].drainPin = drainPin;
  pool[poolID].IDOfSensor = IDOfSensor;
  pool[poolID].SensorpieLenght = SensorpieLenght;
}


//**************Lorra*****************8
float carrierFrequency = 434.0;
float bandwidth = 250.0;
uint8_t spreadingFactor = 7;
uint8_t codingRate = 8;
uint8_t syncWord = 0x33; 
uint8_t outputPower = 20;
uint8_t preambleLength = 12;
uint8_t amplifierGain = 0;
//*************communication***********
String msgFromSink;
String msgToSink;
String buffMsgFromSink;
bool isSended;
bool haveToReset=0;


//***************Execution*******************
int IDOfPool = 0;
int VALVE_SUPPLY_1 = 15;
int VALVE_DRAIN_1 = 27;
int VALVE_SUPPLY_2 = 26;
int VALVE_DRAIN_2 = 25;
int VALVE_SUPPLY_3 = 33;
int VALVE_DRAIN_3 = 32;


//**************CircularQueue************
CircularQueue *buffDataFromSink = createQueue(150);
CircularQueue *buffDataToSink = createQueue(150);
