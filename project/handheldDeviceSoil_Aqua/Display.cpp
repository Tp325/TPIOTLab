#include "Display.h"
float roundToDecimal(float number, int decimalPlaces) {
  float factor = pow(10, decimalPlaces);
  return round(number * factor) / factor;
}
display::display(int addresslcd, int chars, int lines) {
  lcd = new LiquidCrystal_I2C(addresslcd, chars, lines);
}
void display::begin() {
  lcd->init();
  lcd->backlight();
  logo();
  vTaskDelay(2000 / portTICK_PERIOD_MS);
  lcd->clear();
}

void display::logo() {
  print(2, 1, "HANDHELD DEVICES");
}
void display::setting() {
  print(7, 0, "SETTING");
  print(1, 1, "WiFi");
  print(13, 1, "Located");
  print(1, 3, "Mode");
}
void display::settingMode() {
  print(8, 0, "MODE");
  print(1, 1, "Aqua");
  print(13, 1, "Soil");
}
void display::settingLocated() {
  print(7, 0, "LOCATED");
}
void display::settingWifi() {
  print(8, 0, "WIFI");
  if (isWifiConnect) {
    print(0, 1, "WiFi:");
    print(5, 1, wifiUser);
    print(0, 2, "Pass:");
    print(5, 2, wifiPass);
  } else {
    print(2, 1, "not connected");
  }
}
void display::stored() {
  print(8, 0, "STORED");
}
void display::automation() {
  if (deviceMode == 1) {  // aqua
    print(1, 0, "AUTO");
    print(15, 0, nameOfUartSensor);
    if (nameOfUartSensor != "error") {
      printNumber(valueOfUartSensor, 4, 1);
      if (nameOfUartSensor == "SAL") {
        print(16, 3, "ppt");
      }
      if (nameOfUartSensor == "TMP") {
        print(16, 3, "oC");
      }
      if (nameOfUartSensor == "PH") {
        print(16, 3, "H+");
      }
      if (nameOfUartSensor == "NH4") {
        print(16, 3, "ppt");
      }
      if (nameOfUartSensor == "O2") {
        print(16, 3, "%");
      }
    } else {
      printNumber(0.0, 4, 1);
      print(16, 3, "__");
    }
  }
  if (deviceMode == 2) {  // soil
    print(0, 0, "AUTO");
    print(11, 0, "Tmp");
    print(14, 0,soilSensor.tem);
    print(0, 1, "EC:");
    print(4, 1, soilSensor.EC);
    print(11, 1, "Hu:");
    print(14, 1,soilSensor.humidity);
    print(0, 2, "PH:");
    print(4, 2, soilSensor.PH);
    print(11, 2, "N:");
    print(14, 2, soilSensor.nito);
    print(0, 3, "P:");
    print(4, 3, soilSensor.photpho);
    print(11, 3, "K:");
    print(14, 3, soilSensor.kali);
  }
}
void display::calibrate() {
  print(5, 0, "CALIBRATE");
}
void display::home() {
  print(8, 0, "HOME");
  print(1, 1, "Calibrate");
  print(13, 1, "Auto");
  print(1, 3, "Setting");
  print(13, 3, "Stored");
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
void display::watting() {
  print(7, 1, "DONE");
}

void display::screenOn() {
  // Serial.printf("Current page %d\n", currentPage);
  if (isSendPress == 1) {
    lcd->clear();
    watting();
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  } else if (isSendPress == 2) {
    lcd->clear();
    isSendPress = 0;
  } else {
    switch (currentPage) {
      case 0:
        if (isButtonPress == 1) {
          lcd->clear();
          print(x, y, ">");
          isButtonPress = 0;
        }
        home();
        break;
      case 1:
        if (isSellectPress == 1) {
          lcd->clear();
          isSellectPress = 0;
        }
        calibrate();
        break;
      case 2:
        if (isSellectPress == 1) {
          lcd->clear();
          isSellectPress = 0;
        }
        automation();
        break;
      case 3:
        if (isButtonPress == 1) {
          lcd->clear();
          print(x, y, ">");
          isButtonPress = 0;
        }
        if (isSellectPress == 1) {
          lcd->clear();
          isSellectPress = 0;
        }
        setting();
        break;
      case 4:
        if (isSellectPress == 1) {
          lcd->clear();
          isSellectPress = 0;
        }
        stored();
        break;
      case 5:
        if (isSellectPress == 1) {
          lcd->clear();
          isSellectPress = 0;
        }
        settingWifi();
        break;
      case 6:
        if (isSellectPress == 1) {
          lcd->clear();
          isSellectPress = 0;
        }
        settingLocated();
        break;
      case 7:
        if (isButtonPress == 1) {
          lcd->clear();
          print(x, y, ">");
          isButtonPress = 0;
        }
        if (isSellectPress == 1) {
          lcd->clear();
          isSellectPress = 0;
        }
        settingMode();
        break;
    }
  }
}
void display::charOfNumber() {
  lcd->createChar(0, this->full);
  lcd->createChar(1, this->blank);
  lcd->createChar(2, this->ngangtren);
  lcd->createChar(3, this->ngangduoi);
  lcd->createChar(4, this->middle);
  lcd->createChar(5, this->middleduoi);
  lcd->createChar(6, this->middletren);
}
void display::number0(int column, int row) {
  this->charOfNumber();
  lcd->setCursor(column, row);
  lcd->write(char(0));
  lcd->setCursor(column + 1, row);
  lcd->write(char(2));
  lcd->setCursor(column + 2, row);
  lcd->write(char(0));
  lcd->setCursor(column, row + 1);
  lcd->write(char(0));
  lcd->setCursor(column + 1, row + 1);
  lcd->write(char(1));
  lcd->setCursor(column + 2, row + 1);
  lcd->write(char(0));
  lcd->setCursor(column, row + 2);
  lcd->write(char(0));
  lcd->setCursor(column + 1, row + 2);
  lcd->write(char(3));
  lcd->setCursor(column + 2, row + 2);
  lcd->write(char(0));
}
void display::number1(int column, int row) {
  this->charOfNumber();
  this->charOfNumber();
  lcd->setCursor(column, row);
  lcd->write(char(1));
  lcd->setCursor(column + 1, row);
  lcd->write(char(1));
  lcd->setCursor(column + 2, row);
  lcd->write(char(0));
  lcd->setCursor(column, row + 1);
  lcd->write(char(1));
  lcd->setCursor(column + 1, row + 1);
  lcd->write(char(1));
  lcd->setCursor(column + 2, row + 1);
  lcd->write(char(0));
  lcd->setCursor(column, row + 2);
  lcd->write(char(1));
  lcd->setCursor(column + 1, row + 2);
  lcd->write(char(1));
  lcd->setCursor(column + 2, row + 2);
  lcd->write(char(0));
}
void display::number2(int column, int row) {
  this->charOfNumber();
  lcd->setCursor(column, row);
  lcd->write(char(2));
  lcd->setCursor(column + 1, row);
  lcd->write(char(2));
  lcd->setCursor(column + 2, row);
  lcd->write(char(0));
  lcd->setCursor(column, row + 1);
  lcd->write(char(5));
  lcd->setCursor(column + 1, row + 1);
  lcd->write(char(4));
  lcd->setCursor(column + 2, row + 1);
  lcd->write(char(6));
  lcd->setCursor(column, row + 2);
  lcd->write(char(0));
  lcd->setCursor(column + 1, row + 2);
  lcd->write(char(3));
  lcd->setCursor(column + 2, row + 2);
  lcd->write(char(3));
}
void display::number3(int column, int row) {
  this->charOfNumber();
  lcd->setCursor(column, row);
  lcd->write(char(2));
  lcd->setCursor(column + 1, row);
  lcd->write(char(2));
  lcd->setCursor(column + 2, row);
  lcd->write(char(0));
  lcd->setCursor(column, row + 1);
  lcd->write(char(4));
  lcd->setCursor(column + 1, row + 1);
  lcd->write(char(4));
  lcd->setCursor(column + 2, row + 1);
  lcd->write(char(0));
  lcd->setCursor(column, row + 2);
  lcd->write(char(3));
  lcd->setCursor(column + 1, row + 2);
  lcd->write(char(3));
  lcd->setCursor(column + 2, row + 2);
  lcd->write(char(0));
}
void display::number4(int column, int row) {
  this->charOfNumber();
  lcd->setCursor(column, row);
  lcd->write(char(0));
  lcd->setCursor(column + 1, row);
  lcd->write(char(1));
  lcd->setCursor(column + 2, row);
  lcd->write(char(0));
  lcd->setCursor(column, row + 1);
  lcd->write(char(6));
  lcd->setCursor(column + 1, row + 1);
  lcd->write(char(4));
  lcd->setCursor(column + 2, row + 1);
  lcd->write(char(0));
  lcd->setCursor(column, row + 2);
  lcd->write(char(1));
  lcd->setCursor(column + 1, row + 2);
  lcd->write(char(1));
  lcd->setCursor(column + 2, row + 2);
  lcd->write(char(0));
}
void display::number5(int column, int row) {
  this->charOfNumber();
  lcd->setCursor(column, row);
  lcd->write(char(0));
  lcd->setCursor(column + 1, row);
  lcd->write(char(2));
  lcd->setCursor(column + 2, row);
  lcd->write(char(2));
  lcd->setCursor(column, row + 1);
  lcd->write(char(6));
  lcd->setCursor(column + 1, row + 1);
  lcd->write(char(4));
  lcd->setCursor(column + 2, row + 1);
  lcd->write(char(5));
  lcd->setCursor(column, row + 2);
  lcd->write(char(3));
  lcd->setCursor(column + 1, row + 2);
  lcd->write(char(3));
  lcd->setCursor(column + 2, row + 2);
  lcd->write(char(0));
}
void display::number6(int column, int row) {
  this->charOfNumber();
  lcd->setCursor(column, row);
  lcd->write(char(0));
  lcd->setCursor(column + 1, row);
  lcd->write(char(2));
  lcd->setCursor(column + 2, row);
  lcd->write(char(2));
  lcd->setCursor(column, row + 1);
  lcd->write(char(0));
  lcd->setCursor(column + 1, row + 1);
  lcd->write(char(4));
  lcd->setCursor(column + 2, row + 1);
  lcd->write(char(5));
  lcd->setCursor(column, row + 2);
  lcd->write(char(0));
  lcd->setCursor(column + 1, row + 2);
  lcd->write(char(3));
  lcd->setCursor(column + 2, row + 2);
  lcd->write(char(0));
}
void display::number7(int column, int row) {
  this->charOfNumber();
  lcd->setCursor(column, row);
  lcd->write(char(2));
  lcd->setCursor(column + 1, row);
  lcd->write(char(2));
  lcd->setCursor(column + 2, row);
  lcd->write(char(0));
  lcd->setCursor(column, row + 1);
  lcd->write(char(1));
  lcd->setCursor(column + 1, row + 1);
  lcd->write(char(4));
  lcd->setCursor(column + 2, row + 1);
  lcd->write(char(0));
  lcd->setCursor(column, row + 2);
  lcd->write(char(1));
  lcd->setCursor(column + 1, row + 2);
  lcd->write(char(1));
  lcd->setCursor(column + 2, row + 2);
  lcd->write(char(0));
}
void display::number8(int column, int row) {
  this->charOfNumber();
  lcd->setCursor(column, row);
  lcd->write(char(0));
  lcd->setCursor(column + 1, row);
  lcd->write(char(2));
  lcd->setCursor(column + 2, row);
  lcd->write(char(0));
  lcd->setCursor(column, row + 1);
  lcd->write(char(0));
  lcd->setCursor(column + 1, row + 1);
  lcd->write(char(4));
  lcd->setCursor(column + 2, row + 1);
  lcd->write(char(0));
  lcd->setCursor(column, row + 2);
  lcd->write(char(0));
  lcd->setCursor(column + 1, row + 2);
  lcd->write(char(3));
  lcd->setCursor(column + 2, row + 2);
  lcd->write(char(0));
}
void display::number9(int column, int row) {
  this->charOfNumber();
  lcd->setCursor(column, row);
  lcd->write(char(0));
  lcd->setCursor(column + 1, row);
  lcd->write(char(2));
  lcd->setCursor(column + 2, row);
  lcd->write(char(0));
  lcd->setCursor(column, row + 1);
  lcd->write(char(6));
  lcd->setCursor(column + 1, row + 1);
  lcd->write(char(4));
  lcd->setCursor(column + 2, row + 1);
  lcd->write(char(0));
  lcd->setCursor(column, row + 2);
  lcd->write(char(3));
  lcd->setCursor(column + 1, row + 2);
  lcd->write(char(3));
  lcd->setCursor(column + 2, row + 2);
  lcd->write(char(0));
}
void display::printNumber(float inputNumber, int column, int row) {
  switch (int(inputNumber) / 10) {
    case 1:
      this->number1(column, row);
      break;
    case 2:
      this->number2(column, row);
      break;
    case 3:
      this->number3(column, row);
      break;
    case 4:
      this->number4(column, row);
      break;
    case 5:
      this->number5(column, row);
      break;
    case 6:
      this->number6(column, row);
      break;
    case 7:
      this->number7(column, row);
      break;
    case 8:
      this->number8(column, row);
      break;
    case 9:
      this->number9(column, row);
      break;
    case 0:
      this->number0(column, row);
      break;
  }
  switch (int(inputNumber) % 10) {
    case 1:
      this->number1(column + 4, row);
      break;
    case 2:
      this->number2(column + 4, row);
      break;
    case 3:
      this->number3(column + 4, row);
      break;
    case 4:
      this->number4(column + 4, row);
      break;
    case 5:
      this->number5(column + 4, row);
      break;
    case 6:
      this->number6(column + 4, row);
      break;
    case 7:
      this->number7(column + 4, row);
      break;
    case 8:
      this->number8(column + 4, row);
      break;
    case 9:
      this->number9(column + 4, row);
      break;
    case 0:
      this->number0(column + 4, row);
      break;
  }
  lcd->setCursor(column + 7, row + 2);
  lcd->print("O");
  switch (int(inputNumber * 10) % 10) {
    case 1:
      this->number1(column + 8, row);
      break;
    case 2:
      this->number2(column + 8, row);
      break;
    case 3:
      this->number3(column + 8, row);
      break;
    case 4:
      this->number4(column + 8, row);
      break;
    case 5:
      this->number5(column + 8, row);
      break;
    case 6:
      this->number6(column + 8, row);
      break;
    case 7:
      this->number7(column + 8, row);
      break;
    case 8:
      this->number8(column + 8, row);
      break;
    case 9:
      this->number9(column + 8, row);
      break;
    case 0:
      this->number0(column + 8, row);
      break;
  }
}