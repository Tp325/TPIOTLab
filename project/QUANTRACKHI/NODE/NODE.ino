#include "Sensor.h"
#include "Communication.h"
#include "Excution.h"
#include "74HC595.h"
#include "config.h"
#include <SoftwareSerial.h>
#include <ModbusMaster.h>
ModbusMaster node;
Communication communication;
Excution excution;
IC74HC595 ic74HC595;
SoftwareSerial mySerial(18, 17);
DynamicJsonDocument doc3(1024);
Sensor sensor(3, 16, &mySerial, &node);
String firstMsg = "";
TaskHandle_t taskHandleSendToSink = NULL;
TaskHandle_t taskHandleReceiveFromSink = NULL;
TaskHandle_t taskHandleInstallation = NULL;
TaskHandle_t taskHandleAnalize = NULL;
TaskHandle_t taskHandleScanSink = NULL;

void setIOIC0High();
void setIOIC1Low();
void setLed(int redState, int ylState, int blState);
void getDataInNormalMode();
void getDataInCheckingPin();

void setup() {
  //begin
  Serial.begin(9600);
  mySerial.begin(9600);
  manager.begin();
  communication.begin();
  ic74HC595.begin();
  excution.begin();
  //set IO to LOW
  setIOIC1Low();
  ic74HC595.setPin(0, 0, LOW);
  ic74HC595.setPin(0, 7, HIGH);
  setLed(1, 0, 0);
  //set time sleep
  esp_sleep_enable_timer_wakeup(timeToSleep);
  //get data
  if (sensor.readBat() > 3.3 && excution.checkingMode() == "normal") {
    getDataInNormalMode();
  } else if (sensor.readBat() > 3.2 && excution.checkingMode() == "normal") {
    getDataInCheckingPin();
  } else if (sensor.readBat() < 3.2 && excution.checkingMode() == "normal") {
    ic74HC595.setAllLow();
    Serial.println("sleep");
    Serial.flush();
    delay(200);
    esp_deep_sleep_start();
  }
  // start task
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
    setLed(1, 0, 0);
    communication.analize(enviromentParameter);
    vTaskDelay(20 / portTICK_PERIOD_MS);
  }
}
void vtaskSendToSink(void *pvParameters) {
  while (1) {
    communication.sendToSinkWithACK();
    vTaskDelay(20 / portTICK_PERIOD_MS);
  }
}
void vtaskReceiveFromSink(void *pvParameters) {
  while (1) {
    communication.receiveFromSink();
    if (communication.hasMsgFromSink()) {
      setLed(1, 0, 1);
      vTaskDelay(500 / portTICK_PERIOD_MS);
      setLed(1, 0, 0);
    }
    vTaskDelay(20 / portTICK_PERIOD_MS);
  }
}
void vtaskInstallation(void *pvParameters) {
  while (1) {
    communication.installationMode();
    // printf("RSSI: %f,SNR: %f \n", communication.RSSI, communication.SNR);
    if (communication.RSSI < -110 || communication.SNR < -2)
      setLed(0, 0, 0);
    else if (communication.RSSI < -100 || communication.SNR < 0)
      setLed(1, 0, 0);
    else if ((communication.RSSI < -80 && communication.RSSI > -100) || (communication.SNR < 6 && communication.SNR > 0))
      setLed(1, 1, 0);
    else if (communication.RSSI > -80 && communication.SNR > 6)
      setLed(1, 1, 1);
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
      ic74HC595.setAllLow();
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
void setIOIC0High() {
  ic74HC595.setPin(0, 1, 1);
  ic74HC595.setPin(0, 2, 1);
  ic74HC595.setPin(0, 3, 1);
  ic74HC595.setPin(0, 4, 1);
  ic74HC595.setPin(0, 5, 1);
  ic74HC595.setPin(0, 6, 1);
}
void setIOIC1Low() {
  ic74HC595.setPin(0, 1, 0);
  ic74HC595.setPin(0, 2, 0);
  ic74HC595.setPin(0, 3, 0);
  ic74HC595.setPin(0, 4, 0);
  ic74HC595.setPin(0, 5, 0);
  ic74HC595.setPin(0, 6, 0);
}
void setLed(int redState, int ylState, int blState) {
  ic74HC595.setPin(1, 1, redState);
  ic74HC595.setPin(1, 2, ylState);
  ic74HC595.setPin(1, 0, blState);
}
void getDataInNormalMode() {
  setIOIC0High();
  delay(timeStartSensor);
  for (int i = 0; i <= 2; i++) {
    setLed(1, 1, 0);
    enviromentParameter.windSpeed = sensor.getWindSpeed();
    enviromentParameter.windDirection = sensor.getWindDirection();
    enviromentParameter.airTemperature = sensor.getAirTemperature();
    enviromentParameter.airHumidity = sensor.getAirHumidity();
    enviromentParameter.airPressure = sensor.getAirPressure();
    enviromentParameter.vaporPressureDeficit = sensor.getVaporPressureDeficit();
    enviromentParameter.rain = sensor.getRain();
    enviromentParameter.waterTem = sensor.getWaterTem();
    enviromentParameter.pin = sensor.readBat();
    setLed(1, 0, 0);
    delay(500);
  }
  setLed(1, 0, 1);
  setIOIC1Low();
  doc3.clear();
  doc3["SS"] = 0;
  doc3["SID"] = communication.stationID;
  doc3["ID"] = communication.sensorID;
  doc3["WS"] = enviromentParameter.windSpeed;
  doc3["WD"] = enviromentParameter.windDirection;
  doc3["ATem"] = enviromentParameter.airTemperature;
  doc3["AHum"] = enviromentParameter.airHumidity;
  doc3["APr"] = enviromentParameter.airPressure;
  doc3["AVPD"] = enviromentParameter.vaporPressureDeficit;
  doc3["Ra"] = enviromentParameter.rain;
  doc3["WTem"] = enviromentParameter.waterTem;
  doc3["Pin"] = enviromentParameter.pin;
  serializeJson(doc3, firstMsg);
  communication.sendToSink(firstMsg);
  delay(200);
  setLed(1, 0, 0);
}
void getDataInCheckingPin() {
  setLed(1, 0, 1);
  setIOIC1Low();
  doc3.clear();
  doc3["SS"] = 0;
  doc3["SID"] = communication.stationID;
  doc3["ID"] = communication.sensorID;
  doc3["CM"] = "BATC";
  doc3["Pin"] = sensor.readBat();
  serializeJson(doc3, firstMsg);
  communication.sendToSink(firstMsg);
  delay(200);
  setLed(1, 0, 0);
  timeWating = 20000;
}