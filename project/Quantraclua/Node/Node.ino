#include "Sensor.h"
#include "Communication.h"
#include "Excution.h"
#include "config.h"
#include <SoftwareSerial.h>
#include <ModbusMaster.h>

ModbusMaster node;
Communication communication;
Excution excution;
// SoftwareSerial mySerial(16, 17);
DynamicJsonDocument doc3(1024);
Sensor sensor(26, &Serial2, &node);
String firstMsg = "";
TaskHandle_t taskHandleSendToSink = NULL;
TaskHandle_t taskHandleReceiveFromSink = NULL;
TaskHandle_t taskHandleInstallation = NULL;
TaskHandle_t taskHandleAnalize = NULL;
TaskHandle_t taskHandleScanSink = NULL;

void setup() {
  Serial.begin(9600);
  // mySerial.begin(9600);
  Serial2.begin(9600, SERIAL_8N1, 16, 17);
  manager.begin();
  communication.begin();
  excution.begin();
  sensor.begin();
  excution.offSensor(Sensor1);
  excution.offSensor(Sensor2);
  excution.setLed(1, 0, 0);
  esp_sleep_enable_timer_wakeup(timeToSleep);

  if (sensor.getPinVoltage() > 3.3 && excution.checkingMode() == "normal") {
    excution.onSensor(Sensor1);
    excution.onSensor(Sensor2);
    getDataInNormalMode();
    excution.offSensor(Sensor1);
    excution.offSensor(Sensor2);
  }

  else if (sensor.getPinVoltage() > 3.2 && excution.checkingMode() == "normal") {
    getDataInCheckingsensor();
    Serial.println("check sensor");
  }

  else if (sensor.getPinVoltage() < 3.2 && excution.checkingMode() == "normal") {
    excution.offSensor(Sensor1);
    excution.offSensor(Sensor2);
    Serial.println("sleep");
    Serial.flush();
    delay(200);
    esp_deep_sleep_start();
  }

  timeCounter = millis();

  xTaskCreatePinnedToCore(vtaskSendToSink, "taskSendToSink", 4096, NULL, 5, &taskHandleSendToSink, 1);
  xTaskCreatePinnedToCore(vtaskReceiveFromSink, "taskReceiveFromSink", 4096, NULL, 5, &taskHandleReceiveFromSink, 1);
  xTaskCreatePinnedToCore(vtaskInstallation, "taskInstallation", 4096, NULL, 5, &taskHandleInstallation, 1);
  xTaskCreatePinnedToCore(vtaskAnalize, "taskAnalize", 4096, NULL, 5, &taskHandleAnalize, 1);
  xTaskCreatePinnedToCore(vtaskScanSink, "taskScanSink", 4096, NULL, 5, &taskHandleScanSink, 1);
  xTaskCreatePinnedToCore(vtaskBlocking, "vtaskBlocking", 4096, NULL, 5, NULL, 0);
  excution.isChangeMode = 1;
  delay(2000);
  vTaskDelete(NULL);
}

void loop() {
}
void vtaskAnalize(void *pvParameters) {
  while (1) {
    communication.analize(enviromentParameter);
    vTaskDelay(20 / portTICK_PERIOD_MS);
  }
}
void vtaskSendToSink(void *pvParameters) {
  while (1) {
    excution.setLed(1, 0, 0);
    communication.sendToSinkWithACK();
    vTaskDelay(20 / portTICK_PERIOD_MS);
  }
}
void vtaskReceiveFromSink(void *pvParameters) {
  while (1) {
    communication.receiveFromSink();
    if (communication.hasMsgFromSink()) {
      excution.setLed(1, 0, 1);
      vTaskDelay(500 / portTICK_PERIOD_MS);
      excution.setLed(1, 0, 0);
    }
    vTaskDelay(20 / portTICK_PERIOD_MS);
  }
}
void vtaskInstallation(void *pvParameters) {
  while (1) {
    communication.installationMode();
    // printf("RSSI: %f,SNR: %f \n", communication.RSSI, communication.SNR);
    if (communication.RSSI < -110 || communication.SNR < -2)
      excution.setLed(0, 0, 0);
    else if (communication.RSSI < -100 || communication.SNR < 0)
      excution.setLed(1, 0, 0);
    else if ((communication.RSSI < -80 && communication.RSSI > -100) || (communication.SNR < 6 && communication.SNR > 0))
      excution.setLed(1, 1, 0);
    else if (communication.RSSI > -80 && communication.SNR > 6)
      excution.setLed(1, 1, 1);
    timeCounter = millis();
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

void vtaskScanSink(void *pvParameters) {
  while (1) {
    communication.scanNewSink();
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}
void vtaskBlocking(void *pvParameters) {
  while (1) {
    excution.checkingMode();
    if (excution.isChangeMode == 1) {
      Serial.print("mode: ");
      Serial.println(excution.checkingMode());
      if (excution.checkingMode() == "normal") {
        vTaskResume(taskHandleSendToSink);
        vTaskResume(taskHandleReceiveFromSink);
        vTaskResume(taskHandleAnalize);
        vTaskResume(taskHandleScanSink);
        vTaskSuspend(taskHandleInstallation);
      } else if (excution.checkingMode() == "installation") {
        vTaskSuspend(taskHandleSendToSink);
        vTaskSuspend(taskHandleReceiveFromSink);
        vTaskSuspend(taskHandleAnalize);
        vTaskSuspend(taskHandleScanSink);
        vTaskResume(taskHandleInstallation);
      }
      excution.isChangeMode = 0;
    }
    if (communication.haveToReset) {
      ESP.restart();
    }
    if (millis() - timeCounter >= timeWating) {
      Serial.println("sleep");
      Serial.flush();
      vTaskDelay(200 / portTICK_PERIOD_MS);
      esp_deep_sleep_start();
    }
    if (communication.hasMsgFromSink() && communication.hasMsgToSink()) {
      timeCounter = millis();
    }
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}



void getDataInNormalMode() {
  delay(timeStartSensor);
  blinkLed(2, 3);
  sensor.getValueOfSensor();
  excution.offSensor(Sensor1);
  excution.offSensor(Sensor2);
  saveDataToRam();
  blinkLed(3, 1);
  delay(500);
  saveDataToSink();
  delay(200);
  excution.setLed(1, 0, 0);
}

void getDataInCheckingsensor() {
  excution.setLed(1, 0, 1);
  doc3.clear();
  doc3["SS"] = 0;
  doc3["SID"] = communication.stationID;
  doc3["ID"] = communication.sensorID;
  doc3["CM"] = "BATC";
  doc3["sensor"] = sensor.getPinVoltage();
  serializeJson(doc3, firstMsg);
  communication.sendToSink(firstMsg);
  delay(200);
  excution.setLed(1, 0, 0);
  timeWating = 20000;
}

void saveDataToRam() {
  enviromentParameter.Tem = sensor.getTemperature();
  enviromentParameter.Hum = sensor.getHumidity();
  enviromentParameter.PH = sensor.getPH();
  enviromentParameter.EC = sensor.getEC();
  enviromentParameter.Ni = sensor.getNito();
  enviromentParameter.Photpho = sensor.getPhotpho();
  enviromentParameter.Kali = sensor.getKali();
  enviromentParameter.distance = sensor.getDistance();
}

void saveDataToSink() {
  enviromentParameter.pin = sensor.getPinVoltage();
  doc3.clear();
  doc3["SS"] = 0;
  doc3["SID"] = communication.stationID;
  doc3["ID"] = communication.sensorID;
  doc3["Tem"] = enviromentParameter.Tem;
  doc3["Hum"] = enviromentParameter.Hum;
  doc3["EC"] = enviromentParameter.EC;
  doc3["PH"] = enviromentParameter.PH;
  doc3["Ni"] = enviromentParameter.Ni;
  doc3["Pho"] = enviromentParameter.Photpho;
  doc3["Ka"] = enviromentParameter.Kali;
  doc3["Di"] = enviromentParameter.distance;
  doc3["Pin"] = enviromentParameter.pin;
  serializeJson(doc3, firstMsg);
  communication.sendToSink(firstMsg);
}

void blinkLed(int numberOfLed, int time) {
  for (int i = 0; i < time; i++) {
    excution.setLed(1, numberOfLed == 2 ? 1 : 0, numberOfLed == 3 ? 1 : 0);
    delay(200);
    excution.setLed(1, numberOfLed == 2 ? 1 : 0, numberOfLed == 3 ? 1 : 0);
    delay(200);
  }
}
