#include "Excution.h"
Excution::Excution(int S1, int S2,
                   uint8_t switchPin,
                   uint8_t ledRedPin, uint8_t ledYellowPin, uint8_t ledBluePin) {
  this->switchPin = switchPin;
  this->ledRedPin = ledRedPin;
  this->ledYellowPin = ledYellowPin;
  this->ledBluePin = ledBluePin;
}
void Excution::begin() {
  pinMode(Sensor1, OUTPUT);
  pinMode(Sensor2, OUTPUT);
  pinMode(switchPin, INPUT);
  pinMode(ledRedPin, OUTPUT);
  pinMode(ledYellowPin, OUTPUT);
  pinMode(ledBluePin, OUTPUT);
  setLed(1, 0, 0);
}

void Excution::onSensor(int sensor) {
  digitalWrite(sensor, HIGH);
  // digitalWrite(sensor, LOW);

}
void Excution::offSensor(int sensor) {
  digitalWrite(sensor, LOW);
  // digitalWrite(sensor, HIGH);
}
String Excution::checkingMode() {
  if (isSwitchMode != digitalRead(this->switchPin)) {
    isChangeMode = 1;
    isSwitchMode = digitalRead(this->switchPin);
    setLed(1, 1, 1);
    vTaskDelay(500 / portTICK_PERIOD_MS);
    setLed(0, 0, 0);
    vTaskDelay(500 / portTICK_PERIOD_MS);
    setLed(1, 1, 1);
    vTaskDelay(500 / portTICK_PERIOD_MS);
    setLed(0, 0, 0);
    vTaskDelay(500 / portTICK_PERIOD_MS);
    if (digitalRead(this->switchPin) == 1) {
      setLed(1, 0, 0);
    } else {
      setLed(0, 0, 0);
    }
  }
  if (digitalRead(this->switchPin) == 1) {
    return String("normal");
  } else {
    return String("installation");
  }
}
void Excution::setLed(bool stateRedLed, bool stateYellowLed, bool stateBlueLed) {
  this->stateRedLed = stateRedLed;
  this->stateYellowLed = stateYellowLed;
  this->stateBlueLed = stateBlueLed;
  digitalWrite(ledRedPin, this->stateRedLed);
  digitalWrite(ledYellowPin, this->stateYellowLed);
  digitalWrite(ledBluePin, this->stateBlueLed);
}
