#include "Execution.h"
Execution::Execution(int numberOfDevice) {
  this->numberOfDevice = numberOfDevice;
  for (int i = 0; i < numberOfDevice; i++) {
    digitalValue[i] = 0;  // Khởi tạo mảng digitalValue[]
  }
}
void Execution::begin() {
  pinMode(SR_MR_PIN, OUTPUT);
  pinMode(SR_OE_PIN, OUTPUT);
  pinMode(SR_LATCH_PIN, OUTPUT);
  digitalWrite(SR_MR_PIN, register_enable);
  digitalWrite(SR_OE_PIN, output_is_enable);
  SPI.begin();
  setAllLow();
}
void Execution::setbit() {
  SPI.beginTransaction(SPISettings(200000, MSBFIRST, SPI_MODE0));
  digitalWrite(SR_LATCH_PIN, 0);
  for (int i = numberOfDevice - 1; i >= 0; i--) {
    SPI.transfer(digitalValue[i]);
    delay(100);
  }
  digitalWrite(SR_LATCH_PIN, 1);
  SPI.endTransaction();
}
void Execution::setAllLow() {
  for (int i = 0; i < numberOfDevice; i++) {
    digitalValue[i] = 0;
  }
  setbit();
}
void Execution::setAllHigh() {
  for (int i = 0; i < numberOfDevice; i++) {
    digitalValue[i] = 255;
  }
  setbit();
}
void Execution::setPin(int pin, bool value) {
  (value) ? bitSet(digitalValue[pin / 8], pin % 8) : bitClear(digitalValue[pin / 8], pin % 8);
  setbit();
}