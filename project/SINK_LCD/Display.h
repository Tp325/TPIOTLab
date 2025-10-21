#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
extern SemaphoreHandle_t xI2C_Mutex;
class Display {
private:
  LiquidCrystal_I2C* lcd;
  void print(int collum, int row, String msg);
  void print(int collum, int row, float msg);
public:
  Display(uint8_t address = 0x27, int columns = 20, int rows = 4);
  void begin();
  void logo();
  void homePage(float tem, float Sal, float PH, float NH4, float pin);
  void shutDownDisplay();
  void onDisplay();
  void clear();
};

#endif
