#include "Sensor.h"
#include "Communication.h"
#include "Excution.h"
#include "config.h"
#include <SoftwareSerial.h>
Communication communication;
Excution excution;
SoftwareSerial mySerial(16, 17);
DynamicJsonDocument doc3(1024);
Sensor sensor(3, 16, &mySerial, 9600);
String firstMsg = "";
TaskHandle_t taskHandleSendToSink = NULL;
TaskHandle_t taskHandleReceiveFromSink = NULL;
TaskHandle_t taskHandleInstallation = NULL;
TaskHandle_t taskHandleAnalize = NULL;
TaskHandle_t taskHandleScanSink = NULL;
void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  manager.begin();
  communication.begin();
  excution.begin();
  excution.setLed(1, 0, 0);
  esp_sleep_enable_timer_wakeup(timeToSleep);
  if (excution.checkingMode() == "normal") {
    excution.setAllHigh();
    delay(timeStartSensor);
    for (int i = 0; i <= 2; i++) {
      excution.setLed(1, 1, 0);
      enviromentParameter.salinity = sensor.getSalinity();
      enviromentParameter.pH = sensor.getPH();
      enviromentParameter.nH4 = sensor.getNH4();
      enviromentParameter.temperature = sensor.getTemperature();
      enviromentParameter.pin = sensor.readBat();
      excution.setLed(1, 0, 0);
      delay(500);
    }
    excution.setLed(1, 0, 1);
    excution.setAllLow();
    doc3.clear();
    doc3["SS"] = 0;
    doc3["SID"] = communication.stationID;
    doc3["ID"] = communication.sensorID;
    doc3["Sal"] = enviromentParameter.salinity;
    doc3["PH"] = enviromentParameter.pH;
    doc3["NH4"] = enviromentParameter.nH4;
    doc3["Tem"] = enviromentParameter.temperature;
    doc3["Pin"] = enviromentParameter.pin;
    serializeJson(doc3, firstMsg);
    communication.sendToSink(firstMsg);
    delay(200);
    excution.setLed(1, 0, 0);
  }
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
    vTaskDelay(500 / portTICK_PERIOD_MS);
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
