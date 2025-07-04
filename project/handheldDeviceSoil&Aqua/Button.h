#ifndef Button_h
#define Button_h
#include "config.h"
#include <Arduino.h>
class Button {
private:
  int down, up, right, left, ok, send, back;
public:
  Button(int down = 15, int up = 4, int right = 32, int left = 33, int ok = 13, int send = 12, int back = 14);
  // Button(int down = 33, int up = 32, int right = 26, int left = 34, int ok = 27, int send = 18, int back = 5);
  void begin();
  void check();
};
#endif