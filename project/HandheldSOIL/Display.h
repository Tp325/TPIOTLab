#ifndef Display_h
#define Display_h
#include <LiquidCrystal_I2C.h>
#include <Arduino.h>
#include "SensorLib.h"
class display {
private:
  int down, up, right, left, ok, on, back;
  void print(int collum, int row, int msg);
  void print(int collum, int row, String msg);
  void print(int collum, int row, float msg);
  static void interruptHandler();
  int x = 1, y = 1;
  int currentPage = 0;
  void check();
  LiquidCrystal_I2C* lcd;
  static display* instance;
public:
  display(int addresslcd, int chars, int lines);
  void home();                          // page 0;
  void calibrate();                     //page 1;
  void automation(RS485Sensor sensor);  //page 2;
  void setting();                       //page 3;
  void stored();                        //page 4;
  void logo();
  void watting(String msg);
  void begin(int down = 34, int up = 35, int right = 32, int left = 33, int ok = 25, int on = 26, int back = 27);
  void screenOn();
  bool WiFi = 0;
};
#endif
