#include "Display.h"
RS485Sensor sensor(16, 17);
display::display(int addresslcd, int chars, int lines) {
  lcd = new LiquidCrystal_I2C(addresslcd, chars, lines);
}
void display::begin(int down, int up, int right, int left, int ok, int on, int back) {
  pinMode(down, INPUT_PULLUP);
  pinMode(up, INPUT_PULLUP);
  pinMode(right, INPUT_PULLUP);
  pinMode(left, INPUT_PULLUP);
  pinMode(ok, INPUT_PULLUP);
  pinMode(on, INPUT_PULLUP);
  pinMode(back, INPUT_PULLUP);
  // attachInterrupt(digitalPinToInterrupt(down), display::interruptHandler, CHANGE);
  // attachInterrupt(digitalPinToInterrupt(up), display::interruptHandler, CHANGE);
  // attachInterrupt(digitalPinToInterrupt(right), display::interruptHandler, CHANGE);
  // attachInterrupt(digitalPinToInterrupt(left), display::interruptHandler, CHANGE);
  // attachInterrupt(digitalPinToInterrupt(ok), display::interruptHandler, CHANGE);
  // attachInterrupt(digitalPinToInterrupt(on), display::interruptHandler, CHANGE);
  attachInterrupt(digitalPinToInterrupt(back), display::interruptHandler, CHANGE);
  this->down = down;
  this->up = up;
  this->right = right;
  this->left = left;
  this->ok = ok;
  this->on = on;
  this->back = back;
  display::instance = this;
  lcd->init();
  lcd->backlight();
  logo();
  delay(2000);
  lcd->clear();
}
display* display::instance = nullptr;
void display::interruptHandler() {
  if (display::instance != nullptr) {
    display::instance->check();
  }
}
void display::check() {
  if (currentPage == 0 || currentPage == 3) {
    WiFi = 0;
    if (digitalRead(down) == 0) {
      print(x, y, " ");
      y = 3;
      print(x, y, ">");
    }
    if (digitalRead(up) == 0) {
      print(x, y, " ");
      y = 1;
      print(x, y, ">");
    }
    if (digitalRead(right) == 0) {
      print(x, y, " ");
      x = 12;
      print(x, y, ">");
    }
    if (digitalRead(left) == 0) {
      print(x, y, " ");
      x = 0;
      print(x, y, ">");
    }
    if (digitalRead(ok) == 0) {
      lcd->clear();
      if (x == 0 && y == 1)
        currentPage = 1;
      if (x == 12 && y == 1)
        currentPage = 2;
      if (x == 0 && y == 3)
        currentPage = 3;
      if (x == 12 && y == 3)
        currentPage = 4;
    }
    if (digitalRead(back) == 0)
      currentPage = 0;
  }
  if (currentPage == 1 || currentPage == 4) {
    if (digitalRead(back) == 0)
      currentPage = 0;
  }
  if (currentPage == 2) {
    if (digitalRead(back) == 0)
      currentPage = 0;
    if (digitalRead(ok) == 0)
      WiFi = 1;
  }
}
void display::logo() {
  print(2, 1, "HANDHELD DEVICES");
  print(14, 2, "soil");
}
void display::setting() {
  print(7, 0, "SETTING");
  print(2, 1, "WiFi");
  print(12, 1, "Located");
}
void display::automation(RS485Sensor sensor) {
  print(8, 0, "AUTO");
  print(0, 1, "EC:");
  print(4, 1, float(sensor.getEC()) / 1000);
  print(11, 1, "Hu:");
  print(14, 1, float(sensor.getHumidity()) / 10);
  print(0, 2, "PH:");
  print(4, 2, float(sensor.getPH()) / 100);
  print(11, 2, "Ni:");
  print(14, 2, float(sensor.getNito()));
  print(0, 3, "Pho:");
  print(4, 3, float(sensor.getPhotpho()));
  print(11, 3, "Ka:");
  print(14, 3, float(sensor.getKali()));
}
void display::home() {
  print(8, 0, "HOME");
  print(1, 1, "Calibrate");
  print(13, 1, "Auto");
  print(1, 3, "Setting");
  print(13, 3, "Stored");
}
void display::calibrate() {
  print(8, 0, "CALIBRATE");
}
void display::stored() {
  print(8, 0, "STORED");
}
void display::print(int collum, int row, String msg) {
  lcd->setCursor(collum, row);
  lcd->print(msg);
}
void display::print(int collum, int row, int msg) {
  lcd->setCursor(collum, row);
  lcd->print(msg);
}
void display::print(int collum, int row, float msg) {
  lcd->setCursor(collum, row);
  lcd->print(msg);
}
void display::watting(String msg) {
  print(5, 1, "WAITING...");
  print(5, 2, msg);
}

void display::screenOn() {
  switch (currentPage) {
    case 0:
      home();
      break;
    case 1:
      calibrate();
      break;
    case 2:
      automation(sensor);
      break;
    case 3:
      setting();
      break;
    case 4:
      stored();
      break;
  }
}
