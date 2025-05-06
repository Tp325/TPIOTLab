#ifndef Display_h
#define Display_h
#include <LiquidCrystal_I2C.h>
#include <Arduino.h>
#include "SensorLib.h"
#include "config.h"

class display {
private:
  void charOfNumber();
  void number0(int column, int row);
  void number1(int column, int row);
  void number2(int column, int row);
  void number3(int column, int row);
  void number4(int column, int row);
  void number5(int column, int row);
  void number6(int column, int row);
  void number7(int column, int row);
  void number8(int column, int row);
  void number9(int column, int row);
    byte ngangtren[8] = {
    B11111,
    B11111,
    B11111,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
  };
  byte ngangduoi[8] = {
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B11111,
    B11111,
    B11111,
  };
  byte full[8] = {
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
  };
  byte blank[8] = {
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
  };
  byte middle[8] = {
    B00000,
    B00000,
    B00000,
    B11111,
    B11111,
    B00000,
    B00000,
    B00000,
  };
  byte middleduoi[8]{
    B00000,
    B00000,
    B00000,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
  };
  byte middletren[8]{
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B00000,
    B00000,
    B00000,
  };
  void printNumber(float intputNumber, int column, int row);
  void print(int collum, int row, int msg);
  void print(int collum, int row, String msg);
  void print(int collum, int row, float msg);
  void home();            // page 0;
  void calibrate();       //page 1;
  void automation();      //page 2;
  void setting();         //page 3;
  void stored();          //page 4;
  void settingWifi();     //page 5;
  void settingLocated();  //page 6;
  void settingMode();     //page 7;
  void watting();
  LiquidCrystal_I2C* lcd;
public:
  display(int addresslcd, int chars, int lines);
  void logo();
  void begin();
  void screenOn();
};
#endif
