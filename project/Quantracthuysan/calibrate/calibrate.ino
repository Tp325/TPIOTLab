#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3);
void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
}
//PH_2_1  SAL_2_1  TMP_2_1  NH4_2_1
void loop() {
  mySerial.println("NH4_2_1");
  delay(200);
  if (mySerial.available()) {
    Serial.println(137.55 * (mySerial.readStringUntil('\n').toInt() * 5.0 / 1023.0) - 218.2);
  }
  delay(2000);
}

// PH 9.18 H+ - 190   0.92864 V    a.x + b = value //
//PH 7.01 H+ - 233    1.13881 V
//NH4 1ppt - 300    1.46627 V
//NH4 100ppt - 423     2.06745 V