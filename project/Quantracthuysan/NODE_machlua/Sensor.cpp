#include "Sensor.h"

Sensor::Sensor(int BatSense3V3, int BatSense12V,
               HardwareSerial* comunication, long baurate) {
  this->BatSense3V3 = BatSense3V3;
  // this->BatSense12V = BatSense12V;
  this->comunication = comunication;
  this->baurate = baurate;
}
void Sensor::begin() {
  pinMode(BatSense3V3, INPUT);
  // pinMode(BatSense12V, INPUT);
}
float Sensor::median(float numbers[], int size) {
  if (size <= 0) return 0;
  float temp[size];
  for (int i = 0; i < size; i++) {
    temp[i] = numbers[i];
  }
  std::sort(temp, temp + size);
  if (size % 2 == 1) {
    return temp[size / 2];
  } else {
    return (temp[size / 2 - 1] + temp[size / 2]) / 2.0;
  }
}
void Sensor::getValueOfSensor() {
  for (int i = 0; i < 5; i++) {
    Serial.println("*************");
    O2Raw[i] = (getVoltage(sentToSensor("OXY_2_1")) * O2Slope + O2Intercept) < 0.1 ? 0 : (getVoltage(sentToSensor("OXY_2_1")) * O2Slope + O2Intercept);
    Serial.print("O2 voltage: ");
    Serial.println(getVoltage(sentToSensor("OXY_2_1")));
    Serial.print("O2: ");
    Serial.println(O2Raw[i]);
    delay(500);
    Serial.println(" ---- ");
    SalinityRaw[i] = (getVoltage(sentToSensor("SAL_2_1")) * SALSlope + SALIntercept) < 0.1 ? 0 : (getVoltage(sentToSensor("SAL_2_1")) * SALSlope + SALIntercept);
    Serial.print("SAL voltage: ");
    Serial.println(getVoltage(sentToSensor("SAL_2_1")));
    Serial.print("Sal: ");
    Serial.println(SalinityRaw[i]);
    delay(500);
    Serial.println(" ---- ");
    float pHValue = getVoltage(sentToSensor("PH_2_1")) * PHSlope + PHIntercept;
    PHRaw[i] = (pHValue >= 0 && pHValue <= 14) ? pHValue : 0;
    Serial.print("PH voltage: ");
    Serial.println(getVoltage(sentToSensor("PH_2_1")));
    Serial.print("pH: ");
    Serial.println(PHRaw[i]);
    delay(500);
    Serial.println(" ---- ");
    float V = getVoltage(sentToSensor("TMP_2_1"));
    float measured_resistance = (Rref * V) / (5.0 - V);
    float lnR = logf(1000.0 * measured_resistance);
    float invT = Ko + K1 * lnR + K2 * powf(lnR, 3);
    float T = (1.0 / invT) - 273.15;  // °C
    T += 9.0043;
    if (T < 0 || isnan(T)) T = 0;
    TemRaw[i] = T;
    Serial.print("TMP voltage: ");
    Serial.println(V);
    Serial.print("Tem: ");
    Serial.println(TemRaw[i]);
    delay(500);
    Serial.println(" ---- ");
    Vnh4 = getVoltage(sentToSensor("NH4_2_1"));
    Cnh4 = exp((Vnh4 - 1.603128) / 0.143279);
    NH4Raw[i] = Cnh4 > 0 ? Cnh4 : 0.0;
    Serial.print("NH4 voltage: ");
    Serial.println(Vnh4);
    Serial.print("NH4 (mg/L): ");
    Serial.println(NH4Raw[i]);
    delay(500);
  }
}
float Sensor::readVotage(int analogPin) {
  return analogRead(analogPin) * (3.3 / 4095.0);
}

float Sensor::readBat() {
  return readVotage(BatSense3V3) * 1.27;
}
float Sensor ::getVoltage(int RawValue) {
  return (RawValue * 5.0) / 1023.0;
}
int Sensor::sentToSensor(String data) {
  comunication->begin(baurate);
  comunication->println(data);
  delay(400 / portTICK_PERIOD_MS);
  if (comunication->available()) {
    // Serial.println("OK");
    return comunication->readStringUntil('\n').toInt();
  } else {
    Serial.println("ERROR");
    return 0;
  }
}
float Sensor ::getSalinity() {
  return median(SalinityRaw, 5);
}
float Sensor ::getPH() {
  return median(PHRaw, 5);
}
float Sensor ::getNH4() {
  return median(NH4Raw, 5);
}
float Sensor ::getTemperature() {
  return median(TemRaw, 5);
}
float Sensor ::getOxygen() {
  return median(O2Raw, 5);
}
