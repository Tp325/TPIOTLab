#ifndef Function_h
#define Function_h
#include "Arduino.h"
#include "SoftwareSerial.h"
#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
class Sensor {
private:
  int signalOfSensorPin;
public:
  Sensor();
  int readAnalogFromSensor();
  void setup(int signalOfSensorPin);
};

class TransmitToSink {
private:
  int calledId=0;
  int id = 2;
public:
  TransmitToSink();
  void setId(int id);
  void send(Sensor sensor);
  void receive();
  void processDataToSink(Sensor sensor);
  String dataToSend;
};
#endif
