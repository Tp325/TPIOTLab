#include "Function.h"
Sensor sensor;
TransmitToSink node;
void setup() {
  sensor.setup(A0);
  node.setId(1);
  Serial.begin(9600);
}
void loop() {
  node.processDataToSink(sensor);
}
