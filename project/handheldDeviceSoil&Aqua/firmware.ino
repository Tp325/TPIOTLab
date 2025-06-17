#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "Display.h"
#include "Button.h"
#include "communication.h"
#include "CircularQueue.h"
#include "SensorLib.h"
#include "GPS.h"
#include "Storage.h"
#include "config.h"
display screen(0x27, 20, 4);
Button button;
communication communicate;
Storage storage;
RS485Sensor rs485sensor;  //RX 16 TX17
AnalogSensor analogsensor;
GPS satellite(3, 4)
  TaskHandle_t xtaskReadSensorHandle;
void setup() {
  Serial.begin(9600);
  storage.begin();
  screen.begin();
  button.begin();
  communicate.begin();
  size_t freeHeap = xPortGetFreeHeapSize();
  Serial.printf("Free heap size: %u bytes\n", (unsigned int)freeHeap);
  xTaskCreatePinnedToCore(vtaskButton, "taskButton", 2048, NULL, 5, NULL, 1);
  xTaskCreatePinnedToCore(vtaskProcessWiFi, "taskProcessWiFi", 4096, NULL, 5, NULL, 1);
  xTaskCreatePinnedToCore(vtaskSensorDetect, "taskSensorDetect", 2048, NULL, 5, NULL, 1);
  xTaskCreatePinnedToCore(vtaskProcessMQTT, "taskProcessMQTT", 2048, NULL, 5, NULL, 1);
  xTaskCreatePinnedToCore(vtaskDisplay, "taskDisplay", 2048, NULL, 5, NULL, 1);
  xTaskCreatePinnedToCore(vtaskSendToServer, "taskSendToServer", 2048, NULL, 5, NULL, 1);
  xTaskCreatePinnedToCore(vtaskAnalizeDataToSever, "taskAnalizeDataToSever", 2048, NULL, 5, NULL, 1);
  xTaskCreatePinnedToCore(vtaskSaveToEEPROM, "taskSaveToEEPROM", 2048, NULL, 5, NULL, 1);
  xTaskCreatePinnedToCore(vtaskReadSensor, "taskReadSensor", 2048, NULL, 5, &xtaskReadSensorHandle, 1);
  xTaskCreatePinnedToCore(vtaskBlocking, "taskBlocking", 4096, NULL, 5, NULL, 0);
  vTaskDelete(NULL);
}

void loop() {
}
void vtaskSaveToEEPROM(void *pvParameters) {
  while (1) {
    // Serial.println("vtaskSaveToEEPROM running");
    storage.saveToEEPROM();
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}
void vtaskReadSensor(void *pvParameters) {
  while (1) {
    // Serial.println("vtaskReadSensor running");
    if (deviceMode == 1) {
      //**********aqua****************
      // getSalinity(); // unit ppt
      //getPH(); // unit H+
      // getNH4(); // unit
      // getTemperature(); // uint oC
      //*******************************
      analogsensor.begin(9600);
      vTaskDelay(100 / portTICK_PERIOD_MS);
      analogsensor.autoScan();
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
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void vtaskProcessWiFi(void *pvParameters) {
  while (1) {
    // Serial.println("vtaskProcessWiFi running");
    communicate.processWiFi();
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}
void vtaskProcessMQTT(void *pvParameters) {
  while (1) {
    // Serial.println("vtaskProcessMQTT running");
    communicate.processMQTT();
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}
void vtaskBlocking(void *pvParameters) {
  while (1) {
    // reconnect wifi && mqtt reconnnect
    // Serial.println("vtaskBlocking running");
    communicate.reconnectWifi();
    communicate.reconnectMQTT();
    vTaskDelay(3000 / portTICK_PERIOD_MS);
  }
}
void vtaskSendToServer(void *pvParameters) {
  while (1) {
    // Serial.println("vtaskSendToServer running");
    communicate.sendToServer();
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}
void vtaskAnalizeDataToSever(void *pvParameters) {
  while (1) {
    // Serial.println("vtaskAnalizeDataToSever running");
    if (isSendPress == 1 && currentPage == 2) {
      if (deviceMode == 1 && nameOfUartSensor != "eror") {
        if (nameOfUartSensor != "error") {
          if (int(LAT) != 404 && int(LON) != 404) {
            buffmsgToServer = String("{\"is\":0,\"SID\":\"" + String(deviceName) + String("\",\"") + String(nameOfUartSensor) + String("\":") + String(valueOfUartSensor) + String("}"));
            communicate.sendToServer(buffmsgToServer);
            isSendPress = 2;
          }
        }
      }
      if (deviceMode == 2 && soilSensor.tem <= 0.1) {
        if (int(LAT) != 404 && int(LON) != 404) {
          buffmsgToServer = String("{\"is\":0,\"SID\":\"" + String(deviceName) + "\",\"tem\":" + String(soilSensor.tem) + ",\"Humidity\":" + String(soilSensor.humidity) + ",\"EC\":" + String(soilSensor.EC) + ",\"PH\":" + String(soilSensor.PH) + ",\"Ni\":" + String(soilSensor.nito) + ",\"Pho\":" + String(soilSensor.photpho) + ",\"Ka\":" + String(soilSensor.kali) + "}");
          communicate.sendToServer(buffmsgToServer);
          isSendPress = 2;
        }
      }
    }
    vTaskDelay(20 / portTICK_PERIOD_MS);
  }
}
void vtaskButton(void *pvParameters) {
  while (1) {
    // Serial.println("vtaskButton running");
    button.check();
    vTaskDelay(20 / portTICK_PERIOD_MS);
  }
}
void vtaskDisplay(void *pvParameters) {
  while (1) {
    // Serial.println("vtaskDisplay running");
    screen.screenOn();
    if (currentPage == 2) {
      vTaskResume(xtaskReadSensorHandle);
    } else {
      vTaskSuspend(xtaskReadSensorHandle);
    }
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}
void vtaskGPS(void *pvParameters) {
  while (1) {
    satellite.process();
    LAT = satellite.getLAT();
    LON = satellite.getLON();
    vTaskDelay(200 / portTICK_PERIOD_MS);
  }
}

void vtaskSensorDetect(void *parameter) {
  long int screenOnTime = 0;
  while (1) {
    // Serial.println("taskSensorDetect running");
    if (isReadyToSleep == 0) {
      screenOnTime = millis();
      isReadyToSleep = 1;
    }
    if (millis() - screenOnTime >= 2 * 60 * 1000) {
      screen.shutDownDisplay();
    } else {
      screen.onDisplay();
    }
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}
