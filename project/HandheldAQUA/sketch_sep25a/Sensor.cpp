#include "Arduino.h"
#include "Sensor.h"

AnalogSensor::AnalogSensor() {
}
void AnalogSensor::begin(int sig1Pin, int IDPin) {
  this->AnalogInputID = IDPin;
  this->sig1Pin = sig1Pin;
  pinMode(AnalogInputID, INPUT);
  pinMode(sig1Pin, INPUT);
}
//calibrate
void AnalogSensor::getInformationOfSensor() {
  if (getVoltage(AnalogInputID) >= 3.5 && getVoltage(AnalogInputID) <= 3.7) {
    this->nameOfSensor = "Sal";  //1
    this->unit = "ptt";
    this->dataFromSensor = this->getSal(this->sig1Pin);
  } else if (getVoltage(AnalogInputID) >= 1.57 && getVoltage(AnalogInputID) <= 1.65) {
    this->nameOfSensor = "Tmp";  //2
    this->unit = "oC ";
    this->dataFromSensor = this->getTmp(this->sig1Pin);
  }
  // if (getVoltage(AnalogInputID) >= this->parameterOfSensor[0][3]-0.05 && getVoltage(AnalogInputID) < this->parameterOfSensor[0][3]+0.05) {
  //   this->nameOfSensor = "NH4";//3
  //   this->unit = "   ";
  //   this->dataFromSensor = this->getNH4(this->sig1Pin);
  // }
  // else if (getVoltage(AnalogInputID) >= this->parameterOfSensor[0][4] - 0.05 && getVoltage(AnalogInputID) <= this->parameterOfSensor[0][4] + 0.05) {
  //   this->nameOfSensor = "Ph ";  //4
  //   this->unit = "H+ ";
  //   this->dataFromSensor = this->getPh(this->sig1Pin);
  // }
  //  else if (getVoltage(AnalogInputID) >= this->parameterOfSensor[0][5]-0.05 && getVoltage(AnalogInputID) <= this->parameterOfSensor[0][5]+0.05) {
  //   this->nameOfSensor = "Do ";//5
  //   this->unit = "%  ";
  //   this->dataFromSensor = this->getDo(this->sig1Pin);
  // }
  else {
    this->nameOfSensor = "ERR";
    this->unit = "   ";
    this->dataFromSensor = 0;
  }
}
void AnalogSensor::importDataToVariable(int ID, float valueOfSample[2]) {
  switch (ID) {
    case 1:  //SAL
      this->slopeSal = 0;
      this->parameterOfSensor[1][1];  //slope
      this->interceptSal = 0;
      this->parameterOfSensor[2][1];  //intercept
      break;
    case 3:  //NH4
      this->slopeNH4 = 0;
      this->parameterOfSensor[1][3];
      this->interceptNH4 = 0;
      this->parameterOfSensor[2][3];
      break;
    case 4:  //Ph
      this->slopePh = 0;
      this->parameterOfSensor[1][4];
      this->interceptPh = 0;
      this->parameterOfSensor[2][4];
      break;
    case 5:  //Do
      this->slopeDO = 0;
      this->parameterOfSensor[1][5];
      this->interceptDO = 0;
      this->parameterOfSensor[2][5];
      break;
  }
}

//**************************************************************************************************************************
float AnalogSensor::getVoltage(int AnalogInput) {
  float sum = 0;
  for (int i = 1; i <= 10; i++) {
    sum += analogRead(AnalogInput);
    delay(100);
  }
  return ((sum / 10) / 1023.0 * 5.0);
}
float AnalogSensor::getSal(int AnalogInput) {
  //float Sal = getVoltage(AnalogInput) * slopeSal + interceptSal;
  return getVoltage(AnalogInput) * slopeSal + interceptSal-1;
}

float AnalogSensor::getTmp(int AnalogInput) {
  float measuredResistance = (resistance * getVoltage(AnalogInput)) / (5 - getVoltage(AnalogInput));
  float temp = 1.0 / (Ko + K1 * logf(1000 * measuredResistance) + K2 * powf(log(1000.0 * measuredResistance), 3)) - 272.15;
  return temp * slopeTemp + interceptTemp;
}

float AnalogSensor::getPh(int AnalogInput) {
  return getVoltage(AnalogInput) * slopePh + interceptPh;
}

float AnalogSensor::getNH4(int AnalogInput) {
  //float NH4 = exp(((((getVoltage(AnalogInput) - 1.223) / 0.00727) + 168.23) / 23.889) - 7.487);
  //NH4 = NH4 * slopeNH4 + interceptNH4;
  //NH4 = NH4 * 0.3446 + 0.0766;
  return getVoltage(AnalogInput) * slopeNH4 + interceptNH4;
}

float AnalogSensor::getDo(int AnalogInput) {
  //float DO = getVoltage(AnalogInput) * slopeDO + interceptDO;
  // DO = DO * 0.582 + 3.2405;
  // if (DO < 0) {
  //   DO = 0;
  // }
  return getVoltage(AnalogInput) * slopeDO + interceptDO;
}