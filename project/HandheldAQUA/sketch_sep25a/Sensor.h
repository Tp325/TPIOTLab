#ifndef Sensor_h
#define Sensor_h
#include "Arduino.h"
class AnalogSensor {
private:
  const float Ko = 1.02119e-3;
  const float K1 = 2.22468e-4;
  const float K2 = 1.33342e-7;
  uint16_t resistance = 17;
  //TMP
  float slopeTemp = 1.0915;
  float interceptTemp = -3.3994;
  // ph
  float slopePh = -7.78;
  float interceptPh = 16.34;
  // sal 
  float slopeSal = 16.3;
  float interceptSal = 0.0;
  //NH4
  float slopeNH4 = 1;
  float interceptNH4 = 0;
  // do
  float slopeDO = 4.444;
  float interceptDO = -0.4444;
public:
  AnalogSensor();
  int AnalogInputID = A1;
  int sig1Pin = A2;
  String nameOfSensor = "";
  float dataFromSensor;
  String unit = "";
  int ID = 1;
  float parameterOfSensor[3][6];
  int stepOfCalibrate = 0;
  float valueOfSample[2]={};
  void begin(int sig1Pin, int IDPin);
  void getInformationOfSensor();
  void importDataToVariable(int ID,float valueOfSample[2]);
  float getVoltage(int AnalogInput);
  float getSal(int AnalogInput);
  float getTmp(int AnalogInput);
  float getPh(int AnalogInput);
  float getNH4(int AnalogInput);
  float getDo(int AnalogInput);
};
#endif