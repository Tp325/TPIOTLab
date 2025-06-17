#include "config.h"

void setID(int ID) {
  IDOfSensor = ID;
}
//********** Sensor****************
int IDOfSensor = 1;  //doman
// int IDOfSensor=2 //Ph
// int IDOfSensor=3 //NH4
// int IDOfSensor=4 //TMP
// int IDOfSensor=5 //O2
String command;



//*************communication***********
bool sendSensorData;
String msgFromMaster;
String msgToMaster;

//************* CircularQueue*****************
CircularQueue *buffDataFromMaster = createQueue(50);
CircularQueue *buffDataToMaster = createQueue(50);
