#ifndef Function_h
#define Function_h
#include "communication.h"
#include "Display.h"
#include "SensorLib.h"
#include <Arduino.h>
class Function {
private:
  RS485Sensor sensor(2, 3);
  communication server;
  display screen(0x27, 20, 4);
public:
  Function();
  void begin();
  void screenTurnOn();
  void connectToServer();
  void sentToServer();
};
#endif