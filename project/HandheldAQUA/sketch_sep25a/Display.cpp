#include "Display.h"
LiquidCrystal_I2C LCD(0x27, 20, 4);


Number::Number() {
}
void Number::charOfNumber() {
  LCD.createChar(0, this->full);
  LCD.createChar(1, this->blank);
  LCD.createChar(2, this->ngangtren);
  LCD.createChar(3, this->ngangduoi);
  LCD.createChar(4, this->middle);
  LCD.createChar(5, this->middleduoi);
  LCD.createChar(6, this->middletren);
}
void Number::number0(int column, int row) {
  this->charOfNumber();
  LCD.setCursor(column, row);
  LCD.write(char(0));
  LCD.setCursor(column + 1, row);
  LCD.write(char(2));
  LCD.setCursor(column + 2, row);
  LCD.write(char(0));
  LCD.setCursor(column, row + 1);
  LCD.write(char(0));
  LCD.setCursor(column + 1, row + 1);
  LCD.write(char(1));
  LCD.setCursor(column + 2, row + 1);
  LCD.write(char(0));
  LCD.setCursor(column, row + 2);
  LCD.write(char(0));
  LCD.setCursor(column + 1, row + 2);
  LCD.write(char(3));
  LCD.setCursor(column + 2, row + 2);
  LCD.write(char(0));
}
void Number::number1(int column, int row) {
  this->charOfNumber();
  this->charOfNumber();
  LCD.setCursor(column, row);
  LCD.write(char(1));
  LCD.setCursor(column + 1, row);
  LCD.write(char(1));
  LCD.setCursor(column + 2, row);
  LCD.write(char(0));
  LCD.setCursor(column, row + 1);
  LCD.write(char(1));
  LCD.setCursor(column + 1, row + 1);
  LCD.write(char(1));
  LCD.setCursor(column + 2, row + 1);
  LCD.write(char(0));
  LCD.setCursor(column, row + 2);
  LCD.write(char(1));
  LCD.setCursor(column + 1, row + 2);
  LCD.write(char(1));
  LCD.setCursor(column + 2, row + 2);
  LCD.write(char(0));
}
void Number::number2(int column, int row) {
  this->charOfNumber();
  LCD.setCursor(column, row);
  LCD.write(char(2));
  LCD.setCursor(column + 1, row);
  LCD.write(char(2));
  LCD.setCursor(column + 2, row);
  LCD.write(char(0));
  LCD.setCursor(column, row + 1);
  LCD.write(char(5));
  LCD.setCursor(column + 1, row + 1);
  LCD.write(char(4));
  LCD.setCursor(column + 2, row + 1);
  LCD.write(char(6));
  LCD.setCursor(column, row + 2);
  LCD.write(char(0));
  LCD.setCursor(column + 1, row + 2);
  LCD.write(char(3));
  LCD.setCursor(column + 2, row + 2);
  LCD.write(char(3));
}
void Number::number3(int column, int row) {
  this->charOfNumber();
  LCD.setCursor(column, row);
  LCD.write(char(2));
  LCD.setCursor(column + 1, row);
  LCD.write(char(2));
  LCD.setCursor(column + 2, row);
  LCD.write(char(0));
  LCD.setCursor(column, row + 1);
  LCD.write(char(4));
  LCD.setCursor(column + 1, row + 1);
  LCD.write(char(4));
  LCD.setCursor(column + 2, row + 1);
  LCD.write(char(0));
  LCD.setCursor(column, row + 2);
  LCD.write(char(3));
  LCD.setCursor(column + 1, row + 2);
  LCD.write(char(3));
  LCD.setCursor(column + 2, row + 2);
  LCD.write(char(0));
}
void Number::number4(int column, int row) {
  this->charOfNumber();
  LCD.setCursor(column, row);
  LCD.write(char(0));
  LCD.setCursor(column + 1, row);
  LCD.write(char(1));
  LCD.setCursor(column + 2, row);
  LCD.write(char(0));
  LCD.setCursor(column, row + 1);
  LCD.write(char(6));
  LCD.setCursor(column + 1, row + 1);
  LCD.write(char(4));
  LCD.setCursor(column + 2, row + 1);
  LCD.write(char(0));
  LCD.setCursor(column, row + 2);
  LCD.write(char(1));
  LCD.setCursor(column + 1, row + 2);
  LCD.write(char(1));
  LCD.setCursor(column + 2, row + 2);
  LCD.write(char(0));
}
void Number::number5(int column, int row) {
  this->charOfNumber();
  LCD.setCursor(column, row);
  LCD.write(char(0));
  LCD.setCursor(column + 1, row);
  LCD.write(char(2));
  LCD.setCursor(column + 2, row);
  LCD.write(char(2));
  LCD.setCursor(column, row + 1);
  LCD.write(char(6));
  LCD.setCursor(column + 1, row + 1);
  LCD.write(char(4));
  LCD.setCursor(column + 2, row + 1);
  LCD.write(char(5));
  LCD.setCursor(column, row + 2);
  LCD.write(char(3));
  LCD.setCursor(column + 1, row + 2);
  LCD.write(char(3));
  LCD.setCursor(column + 2, row + 2);
  LCD.write(char(0));
}
void Number::number6(int column, int row) {
  this->charOfNumber();
  LCD.setCursor(column, row);
  LCD.write(char(0));
  LCD.setCursor(column + 1, row);
  LCD.write(char(2));
  LCD.setCursor(column + 2, row);
  LCD.write(char(2));
  LCD.setCursor(column, row + 1);
  LCD.write(char(0));
  LCD.setCursor(column + 1, row + 1);
  LCD.write(char(4));
  LCD.setCursor(column + 2, row + 1);
  LCD.write(char(5));
  LCD.setCursor(column, row + 2);
  LCD.write(char(0));
  LCD.setCursor(column + 1, row + 2);
  LCD.write(char(3));
  LCD.setCursor(column + 2, row + 2);
  LCD.write(char(0));
}
void Number::number7(int column, int row) {
  this->charOfNumber();
  LCD.setCursor(column, row);
  LCD.write(char(2));
  LCD.setCursor(column + 1, row);
  LCD.write(char(2));
  LCD.setCursor(column + 2, row);
  LCD.write(char(0));
  LCD.setCursor(column, row + 1);
  LCD.write(char(1));
  LCD.setCursor(column + 1, row + 1);
  LCD.write(char(4));
  LCD.setCursor(column + 2, row + 1);
  LCD.write(char(0));
  LCD.setCursor(column, row + 2);
  LCD.write(char(1));
  LCD.setCursor(column + 1, row + 2);
  LCD.write(char(1));
  LCD.setCursor(column + 2, row + 2);
  LCD.write(char(0));
}
void Number::number8(int column, int row) {
  this->charOfNumber();
  LCD.setCursor(column, row);
  LCD.write(char(0));
  LCD.setCursor(column + 1, row);
  LCD.write(char(2));
  LCD.setCursor(column + 2, row);
  LCD.write(char(0));
  LCD.setCursor(column, row + 1);
  LCD.write(char(0));
  LCD.setCursor(column + 1, row + 1);
  LCD.write(char(4));
  LCD.setCursor(column + 2, row + 1);
  LCD.write(char(0));
  LCD.setCursor(column, row + 2);
  LCD.write(char(0));
  LCD.setCursor(column + 1, row + 2);
  LCD.write(char(3));
  LCD.setCursor(column + 2, row + 2);
  LCD.write(char(0));
}
void Number::number9(int column, int row) {
  this->charOfNumber();
  LCD.setCursor(column, row);
  LCD.write(char(0));
  LCD.setCursor(column + 1, row);
  LCD.write(char(2));
  LCD.setCursor(column + 2, row);
  LCD.write(char(0));
  LCD.setCursor(column, row + 1);
  LCD.write(char(6));
  LCD.setCursor(column + 1, row + 1);
  LCD.write(char(4));
  LCD.setCursor(column + 2, row + 1);
  LCD.write(char(0));
  LCD.setCursor(column, row + 2);
  LCD.write(char(3));
  LCD.setCursor(column + 1, row + 2);
  LCD.write(char(3));
  LCD.setCursor(column + 2, row + 2);
  LCD.write(char(0));
}
void Number::print(float inputNumber, int column, int row) {
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
  LCD.setCursor(column + 7, row + 2);
  LCD.print("O");
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
AnalogSensor analogSensor;
Display::Display() {
}
void Display::begin() {
  LCD.init();
  LCD.backlight();
  this->logo();
  analogSensor.begin(A1,A0);
  delay(2000);
  LCD.clear();
}
void Display::logo() {
  LCD.setCursor(2, 1);
  LCD.print("HANDHELD DEVICES");
  LCD.setCursor(14, 2);
  LCD.print("AQUA");
}
void Display::waitting() {
  LCD.setCursor(5, 1);
  LCD.print("WAITTING...");
  delay(1000);
}
void Display::home(Pointer pointer) {
  pointer.createPointer();
  LCD.setCursor(8, 0);
  LCD.print("HOME");
  LCD.setCursor(1, 1);
  LCD.print("Calibrate");
  LCD.setCursor(13, 1);
  LCD.print("Auto");
  LCD.setCursor(1, 3);
  LCD.print("Setting");
  LCD.setCursor(13, 3);
  LCD.print("Stored");
}
void Display::selectSensor(Pointer pointer) {
  pointer.buttomStatusForSelectSensor();
  LCD.setCursor(6, 0);
  LCD.print("CALIBRATE");
  switch (analogSensor.ID) {
    case 1:
      LCD.setCursor(9, 2);
      LCD.print("SAL");
      break;
    case 2:
      LCD.setCursor(9, 2);
      LCD.print("TMP");
      break;
    case 3:
      LCD.setCursor(9, 2);
      LCD.print("NH4");
      break;
    case 4:
      LCD.setCursor(9, 2);
      LCD.print("Ph ");
      break;
    case 5:
      LCD.setCursor(9, 2);
      LCD.print("Do ");
      break;
  }
}
void Display::calibrate(Pointer pointer) {
  if (analogSensor.stepOfCalibrate == 0) {
    pointer.buttomStatusForSelectSensor();
    this->selectSensor(pointer);
    LCD.setCursor(1, 2);
    LCD.print(analogSensor.stepOfCalibrate);
    LCD.setCursor(2, 3);
    LCD.print("/3");
  } else {
    Number number;
    analogSensor.getInformationOfSensor();
    pointer.buttomStatusForCalibrate();
    LCD.setCursor(1, 0);
    LCD.print("CALIBRATE");
    LCD.setCursor(15, 0);
    LCD.print(analogSensor.nameOfSensor);
    if (analogSensor.stepOfCalibrate == 1) {
      number.print(analogSensor.valueOfSample[0], 5, 1);
    }
    if (analogSensor.stepOfCalibrate == 2) {
      number.print(analogSensor.valueOfSample[1], 5, 1);
    }
    LCD.setCursor(16, 3);
    LCD.print(analogSensor.unit);
    LCD.setCursor(1, 2);
    LCD.print(analogSensor.stepOfCalibrate);
    LCD.setCursor(2, 3);
    LCD.print("/3");
    if (analogSensor.stepOfCalibrate == 3) {
      delay(500);
      LCD.clear();
      this->waitting();
      analogSensor.importDataToVariable(analogSensor.ID, analogSensor.valueOfSample);
      analogSensor.stepOfCalibrate = 0;
      LCD.clear();
    }
  }
}
void Display::Auto() {
  analogSensor.getInformationOfSensor();
  Number number;
  LCD.setCursor(1, 0);
  LCD.print("AUTO");
  LCD.setCursor(15, 0);
  LCD.print(analogSensor.nameOfSensor);
  number.print(analogSensor.dataFromSensor, 4, 1);
  LCD.setCursor(16, 3);
  LCD.print(analogSensor.unit);
}
Pointer::Pointer() {
}
void Pointer::begin(int stepColumn, int stepRow) {
  this->stepColumn = stepColumn;
  this->stepRow = stepRow;
  pinMode(this->right, INPUT_PULLUP);
  pinMode(this->left, INPUT_PULLUP);
  pinMode(this->top, INPUT_PULLUP);
  pinMode(this->bottom, INPUT_PULLUP);
  pinMode(this->enter, INPUT_PULLUP);
  pinMode(this->back, INPUT_PULLUP);
}

void Pointer::buttomStatusForHome() {
  if (digitalRead(this->right) == 0 && millis() - this->waitTime >= 250 && this->presentColumn + this->stepColumn <= 19) {
    LCD.setCursor(this->presentColumn, this->presentRow);
    LCD.print(" ");
    this->presentColumn += this->stepColumn;
    this->waitTime = millis();
  }
  if (digitalRead(this->left) == 0 && millis() - this->waitTime >= 250 && this->presentColumn - this->stepColumn >= 0) {
    LCD.setCursor(this->presentColumn, this->presentRow);
    LCD.print(" ");
    this->presentColumn -= this->stepColumn;
    this->waitTime = millis();
  }
  if (digitalRead(this->top) == 0 && millis() - this->waitTime >= 250 && this->presentRow - this->stepRow >= 0) {
    LCD.setCursor(this->presentColumn, this->presentRow);
    LCD.print(" ");
    this->presentRow -= this->stepRow;
    this->waitTime = millis();
  }
  if (digitalRead(this->bottom) == 0 && millis() - this->waitTime >= 250 && this->presentRow + this->stepRow <= 3) {
    LCD.setCursor(this->presentColumn, this->presentRow);
    LCD.print(" ");
    this->presentRow += this->stepRow;
    this->waitTime = millis();
  }
  if (digitalRead(this->enter) == 0 && millis() - this->waitTime >= 250) {
    LCD.clear();
    if (this->presentColumn == 0 && this->presentRow == 1) {
      this->page = 1;
      delay(200);
    }
    if (this->presentColumn == 12 && this->presentRow == 1) {
      this->page = 2;
    }
    if (this->presentColumn == 0 && this->presentRow == 3) {
      this->page = 3;
    }
    if (this->presentColumn == 12 && this->presentRow == 3) {
      this->page = 4;
    }
    this->waitTime = millis();
  }
}

void Pointer::buttomStatusForCalibrate() {
  if (digitalRead(this->top) == 0 && millis() - this->waitTime >= 50) {
    if (analogSensor.stepOfCalibrate == 1) {
      analogSensor.valueOfSample[0] += 0.1;
    }
    if (analogSensor.stepOfCalibrate == 2) {
      analogSensor.valueOfSample[1] += 0.1;
    }
    this->waitTime = millis();
  }
  if (digitalRead(this->bottom) == 0 && millis() - this->waitTime >= 50 && analogSensor.valueOfSample[0] - 0.1 >= -0.1 && analogSensor.valueOfSample[1] - 0.1 >= -0.1) {
    if (analogSensor.stepOfCalibrate == 1) {
      analogSensor.valueOfSample[0] -= 0.1;
    }
    if (analogSensor.stepOfCalibrate == 2) {
      analogSensor.valueOfSample[1] -= 0.1;
    }
    this->waitTime = millis();
  }
  if (digitalRead(this->enter) == 0 && millis() - this->waitTime >= 200) {
    analogSensor.stepOfCalibrate++;
    this->waitTime = millis();
  }
}
void Pointer::buttomFuntion() {
  if (digitalRead(this->back) == 0 && millis() - this->waitTime >= 250 && this->page > 0) {
    LCD.clear();
    this->page = 0;
    analogSensor.stepOfCalibrate = 0;
    this->waitTime = millis();
  }
}
void Pointer::buttomStatusForSelectSensor() {
  if (digitalRead(this->top) == 0 && millis() - this->waitTime >= 250 && analogSensor.ID + 1 <= 5) {
    analogSensor.ID++;
    this->waitTime = millis();
  }
  if (digitalRead(this->bottom) == 0 && millis() - this->waitTime >= 250 && analogSensor.ID - 1 >= 1) {
    analogSensor.ID--;
    this->waitTime = millis();
  }
  if (digitalRead(this->enter) == 0 && millis() - this->waitTime >= 250) {
    analogSensor.parameterOfSensor[0][analogSensor.ID] = analogSensor.getVoltage(analogSensor.AnalogInputID);
    analogSensor.stepOfCalibrate++;
    LCD.clear();
    this->waitTime = millis();
  }
}
void Pointer::createPointer(int column, int row) {
  LCD.createChar(0, this->arrow);
  LCD.setCursor(column, row);
  LCD.write(0);
}
void Pointer::createPointer() {
  LCD.createChar(0, this->arrow);
  LCD.setCursor(this->presentColumn, this->presentRow);
  LCD.write(0);
}