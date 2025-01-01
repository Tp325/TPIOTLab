#include "Vernier.h"
Vernier::Vernier(int SIG1Pin, int Sig2Pin) {
  pinMode(VresPin, INPUT);
  pinMode(IDPin, INPUT);
  pinMode(SIG1Pin, INPUT);
  pinMode(Sig2Pin, INPUT);
}
void Vernier::i2cCommunicateToSensor(int sensorAddress, int dataForSend, int possition) {
  pinMode(VresPin, OUTPUT);
  pinMode(IDPin, OUTPUT);
  Wire.begin();
  Wire.beginTransmission(sensorAddress);
  Wire.write(dataForSend);
  Wire.endTransmission();
  int i = 1;
  int sizeOfSensor = Wire.requestFrom(sensorAddress, 32);
  while (sizeOfSensor > 1) {
    sizeOfSensor = Wire.available();
    sensorData[i + possition] = Wire.read();
    i++;
  }
  // for (int i = 0; i <= 127; i++) {
  //   Serial.print(sensorData[i]);
  //   Serial.print("\t");
  //   Serial.println(char(sensorData[i]));
  // }
}
void Vernier::getInformationOfSensor() {
  for (int i = 0; i < 16; i++) {
    if (i < 7) {
      this->sensorUnit[i] = char(sensorData[i + 83 + (page)*19]);
    }
    if (i < 11) {
      this->shortName[i] = char(sensorData[i + 29]);
    }
    if (i < 16) {
      this->sensorName[i] = char(sensorData[i + 9]);
    }
  }
  shortName[11] = "\0";
  sensorName[16] = "\0";
  sensorUnit[7] = "\0";
  // Intercept starts at 71 for page 1, 90 for p2, and 109 for p3
  //Determines intercept:
  for (int i = 0; i < 4; i++) {
    floatbyte[i] = sensorData[i + 71 + (page)*19];
  }
  memcpy(&this->intercept, floatbyte, sizeof(float));
  // slope starts at 75 for page 1, 94 for p2, and 113 for p3
  //Determines slope:
  for (int i = 0; i < 4; i++) {
    floatbyte[i] = sensorData[i + 75 + (page * 19)];
  }
  memcpy(&this->slope, floatbyte, sizeof(float));
}
void Vernier::autoID() {
  float voltageID = 0;
  this->sensorNumber = 0;
  for (int i = 1; i <= 3; i++) {
    voltageID += analogRead(IDPin);
  }
  voltageID = (voltageID / 3) * 5 / 1023.5;
  if (voltageID > 4.32 & voltageID < 4.40) {
    slope = 1;
    intercept = 0;
    sensorNumber = 1;  // CO2
  }
  if (voltageID > 4.50 & voltageID < 4.59) {
    slope = 1;
    intercept = 0;
    sensorNumber = 2;  // Oxygen
  }
  if (voltageID > 2.38 & voltageID < 2.63) {
    slope = 1;
    intercept = 0;
    sensorNumber = 3;  //Temperature
  }
  if (sensorNumber != 0) {
    int i = 0;
    for (i = 0; i < 14; i++) {
      sensorData[9 + i] = resistorIDInfo[sensorNumber][i];
    }
    //read ShortName char string into array in the right spots, characters 15-24
    for (i = 0; i < 10; i++)  //
    {
      sensorData[29 + i] = resistorIDInfo[sensorNumber][14 + i];
    }
    // read Units char string into array in the right spots, characters 25-32
    for (i = 0; i < 7; i++)  //
    {
      sensorData[83 + i] = resistorIDInfo[sensorNumber][24 + i];
    }
  } else {
    //Read Data for the first time
    this->i2cCommunicateToSensor(sensorAddress, 0x0, 0);
    delay(50);
    //Read Data for the second time
    this->i2cCommunicateToSensor(sensorAddress, 0x20, 32);
    delay(50);
    //Read Data for the third time
    this->i2cCommunicateToSensor(sensorAddress, 0x40, 64);
    delay(50);
    //Read Date for the forth time
    this->i2cCommunicateToSensor(sensorAddress, 0x60, 96);
    delay(50);
    //Import the data read by i2c to variable
    sensorNumber = sensorData[2];
    //Determines the  calibration page:
    page = sensorData[70];
    //turn off i2c
    pinMode(A4, INPUT);
    pinMode(A5, INPUT);
  }
  this->getInformationOfSensor();
}
float Vernier::readSensor() {
  int numberAveraged = 10;
  int sum = 0;
  float voltage;
  for (int i = 0; i < numberAveraged; i++) {
    {
      sum += analogRead(SIG1Pin);
    }
  }
  voltage = (float(sum) / numberAveraged) / 1024.0 * 5.0;
  switch (this->sensorNumber) {

    case 97:  //Ethanol Sensor
      return intercept * pow(voltage, slope);
      break;
    case 38:  //ISEs, CA(38), NH4(39), NO3(40), Cl(41) sensor
    case 36:
    case 40:
    case 41:
    case 42:
    case 113:
      return 137.55 * voltage - 168.2;
      break;
    case 61:  // sanility
      return int(0 + voltage * 16.3) == 0 ? 0 : 0 + voltage * 16.3;
    case 10:  // Temperature sensor
      float logR;
      float resistor = 15000;  //15k resistor
      long resistance = (resistor * voltage) / (5.0 - voltage);
      logR = log(resistance);
      return (1 / (0.00102119 + (0.000222468 * logR + (0.000000133342 * logR * logR * logR)))) - 273.15;
      break;
    default:
      return intercept + voltage * slope;
      break;
  }
}