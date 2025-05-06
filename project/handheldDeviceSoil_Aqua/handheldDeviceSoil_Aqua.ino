#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "Display.h"
#include "Button.h"
#include "communication.h"
#include "SensorLib.h"
#include "config.h"
display screen(0x27, 20, 4);
Button button;
communication communicate;
RS485Sensor rs485sensor;
AnalogSensor analogsensor;
TaskHandle_t xtaskReadSensorHandle;
void setup() {
  Serial.begin(9600);
  screen.begin();
  button.begin();
  communicate.begin();
  xTaskCreate(vtaskDisplay, "taskDisplay", 10000, NULL, 5, NULL);
  xTaskCreate(vtaskButton, "taskButton", 10000, NULL, 5, NULL);
  xTaskCreate(vtaskProcessWiFi, "taskProcessWiFi", 10000, NULL, 5, NULL);
  xTaskCreate(vtaskProcessMQTT, "taskProcessMQTT", 10000, NULL, 5, NULL);
  xTaskCreate(vtaskSendToServer, "taskSendToServer", 10000, NULL, 5, NULL);
  xTaskCreate(vtaskReadSensor, "taskReadSensor", 10000, NULL, 5, &xtaskReadSensorHandle);
  vTaskDelete(NULL);
}

void loop() {
}

void vtaskReadSensor(void *pvParameters) {
  while (1) {
    if (deviceMode == 1) {
      //**********aqua****************
      // getSalinity(); // unit ppt
      //getPH(); // unit H+
      // getNH4(); // unit
      // getTemperature(); // uint oC
      //*******************************
      analogsensor.begin(9600);
      vTaskDelay(100 / portTICK_PERIOD_MS);
      analogsensor.automationScan();
      if (nameOfUartSensor == "SAL") {
        valueOfUartSensor = analogsensor.getSalinity();
      }
      if (nameOfUartSensor == "TMP") {
        valueOfUartSensor = analogsensor.getTemperature();
      }
      if (nameOfUartSensor == "PH") {
        valueOfUartSensor = analogsensor.getPH();
      }
      if (nameOfUartSensor == "NH4") {
        valueOfUartSensor = analogsensor.getNH4();
      }
      if (nameOfUartSensor == "O2") {
        // analogsensor.
      }
    }
    if (deviceMode == 2) {
      //*******************soil************
      // getTemperature();  //unit oC
      // getHumidity();     //unit %
      // getPH();           //unit H+
      // getNito();         //unit mg/kg
      // getPhotpho();      //unit mg/kg
      // getKali();         //unit mg/kg
      // getEC();           //unit us/cm
      //**************************************
      // rs485sensor.begin(115200);
      rs485sensor.begin(9600);
      vTaskDelay(100 / portTICK_PERIOD_MS);
      soilSensor.tem = rs485sensor.getTemperature();
      vTaskDelay(100 / portTICK_PERIOD_MS);
      soilSensor.humidity = rs485sensor.getHumidity();
      vTaskDelay(100 / portTICK_PERIOD_MS);
      soilSensor.EC = rs485sensor.getEC();
      vTaskDelay(100 / portTICK_PERIOD_MS);
      soilSensor.PH = rs485sensor.getPH();
      vTaskDelay(100 / portTICK_PERIOD_MS);
      soilSensor.nito = rs485sensor.getNito();
      vTaskDelay(100 / portTICK_PERIOD_MS);
      soilSensor.photpho = rs485sensor.getPhotpho();
      vTaskDelay(100 / portTICK_PERIOD_MS);
      soilSensor.kali = rs485sensor.getKali();
      vTaskDelay(100 / portTICK_PERIOD_MS);
    }
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

void vtaskProcessWiFi(void *pvParameters) {
  while (1) {
    communicate.processWiFi();
    vTaskDelay(20 / portTICK_PERIOD_MS);
  }
}

void vtaskProcessMQTT(void *pvParameters) {
  while (1) {
    communicate.processMQTT();
    vTaskDelay(20 / portTICK_PERIOD_MS);
  }
}
void vtaskSendToServer(void *pvParameters) {
  while (1) {
    if (isSendPress == 1 && currentPage == 2) {
      if (deviceMode == 1 && nameOfUartSensor != "eror") {
        if (nameOfUartSensor != "error") {
          msgToMQTT = String("{\"ID\":\"" + String(deviceName) + String("\",\"") + String(nameOfUartSensor) + String("\":") + String(valueOfUartSensor) + String("}"));
          communicate.sendToMqtt(msgToMQTT, "HANDHELDDEVICESOIL");
        }
      }
      if (deviceMode == 2 && soilSensor.tem != 0.0) {
        msgToMQTT = String("{\"ID\":\"" + String(deviceName) + "\",\"tem\":" + String(soilSensor.tem) + ",\"Humidity\":" + String(soilSensor.humidity) + ",\"EC\":" + String(soilSensor.EC) + ",\"PH\":" + String(soilSensor.PH) + ",\"Ni\":" + String(soilSensor.nito) + ",\"Pho\":" + String(soilSensor.photpho) + ",\"Ka\":" + String(soilSensor.kali) + "}");
        communicate.sendToMqtt(msgToMQTT, "HANDHELDDEVICESOIL");
      }
      isSendPress = 2;
    }
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}
void vtaskButton(void *pvParameters) {
  while (1) {
    button.check();
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}
void vtaskDisplay(void *pvParameters) {
  while (1) {
    screen.screenOn();
    if (currentPage == 2) {
      vTaskResume(xtaskReadSensorHandle);
    } else {
      vTaskSuspend(xtaskReadSensorHandle);
    }
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}
