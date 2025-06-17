#include "config.h"


Timmer timmer;
//********** Sensor****************
extern int IDOfSensor;

//*************Expanse module*************
int SR_LATCH_PIN = 21;  //Do22
int SR_SCK_PIN = 12;    //Clock
int SR_MOSI_PIN = 11;   //SerialDatapin

//**************Lorra*****************8
float carrierFrequency = 434.0;
float bandwidth = 250.0;
uint8_t spreadingFactor = 7;
uint8_t codingRate = 8;
uint8_t syncWord = 0x34;
uint8_t outputPower = 20;
uint8_t preambleLength = 12;
uint8_t amplifierGain = 0;

//***************MQTT*************
bool isWifiConnect = 0;
const char *mqtt_server = "103.221.220.183";
const int mqtt_port = 1883;
const char *mqtt_user = "api1@Iotlab";
const char *mqtt_pass = "Iotlab@2023";
String topicReceive = "receiveAquastationDemo";
// String topicSend = "sendAquastationDemo";
String topicSend = "HondakiengiangAqua";


//**************CircularQueue************
CircularQueue *buffDataFromSink = createQueue(50);
CircularQueue *buffDataToSink = createQueue(50);
CircularQueue *buffDataFromNode = createQueue(50);
CircularQueue *buffDataToNode = createQueue(50);
CircularQueue *buffDataToServer = createQueue(50);
CircularQueue *buffDataFromServer = createQueue(50);

//*******************sensor***************
String sensorName;
sensor SAL = { 16.3, 0.0 };
sensor PH = { -8.663, 16.6645 };
sensor NH4 = { 136.86, -186.2838 };
sensor TMP = { 1.0915, -3.3994 };
sensor O2 = { 4.444, -0.4444 };

//*************communication***********
String msgFromSink;
String msgToSink;
String buffMsgToSink;
String msgToNode;
String msgFromNode;
String msgFromServer;
String msgToServer;
bool isSended;