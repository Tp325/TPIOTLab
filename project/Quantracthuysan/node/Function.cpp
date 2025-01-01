#include "Function.h"
#include "Arduino.h"

Sensor::Sensor() {
}

void Sensor::setup(int signalOfSensorPin) {
  this->signalOfSensorPin = signalOfSensorPin;
  pinMode(signalOfSensorPin, INPUT);
}

int Sensor::readAnalogFromSensor() {
  int analogPin = this->signalOfSensorPin;
  return analogRead(analogPin);
}

TransmitToSink::TransmitToSink() {
}
SoftwareSerial community(10, 11);

DynamicJsonDocument Stored(1024);
void TransmitToSink::receive() {
  deserializeJson(Stored, community.readString());
  if (this->id == Stored["Id"].as<int>()) {
    this->calledId = Stored["Id"].as<int>();
  } else {
    calledId=0;
  }
  delay(100);
}
void TransmitToSink::setId(int id) {
  community.begin(9600);
  Serial.begin(9600);
  this->id = id;
}
void TransmitToSink::send(Sensor sensor) {
  DynamicJsonDocument dataSendToSink(1024);
  if (this->id == this->calledId) {
    dataSendToSink["Id"] = this->id;
    dataSendToSink["Data"] = sensor.readAnalogFromSensor();
  }
  Serial.println(calledId);
  serializeJson(dataSendToSink, dataToSend);
}

void TransmitToSink::processDataToSink(Sensor sensor) {
  unsigned int waitTime = millis();
  while (!community.available()) {
    if (millis() - waitTime >= 2000) {
      break;
    }
  }
  this->receive();
  this->send(sensor);
  if (this->id==this->calledId) {
    community.println(dataToSend);
    Serial.println(dataToSend);
  }
  delay(200);
}