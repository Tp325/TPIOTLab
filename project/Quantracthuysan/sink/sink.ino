#include "Communication.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
Communication communication;
void setup() {
  Serial.begin(9600);
  communication.begin();
  xTaskCreatePinnedToCore(vtaskSendToNode, "taskSendToNode", 4096, NULL, 5, NULL, 1);
  xTaskCreatePinnedToCore(vtaskSendToSever, "taskSendToSever", 4096, NULL, 5, NULL, 1);
  xTaskCreatePinnedToCore(vtaskReceiveFromNode, "taskReceiveFromNode", 4096, NULL, 5, NULL, 1);
  xTaskCreatePinnedToCore(vtaskBlocking, "taskBlocking", 4096, NULL, 5, NULL, 0);
  xTaskCreatePinnedToCore(vtaskProcessWiFi, "taskProcessWiFi", 4096, NULL, 5, NULL, 0);
  xTaskCreatePinnedToCore(vtaskProcessMQTT, "taskProcessMQTT", 4096, NULL, 5, NULL, 0);
  vTaskDelete(NULL);
}

void loop() {
}
void vtaskSendToNode(void *pvParameters) {
  while (1) {
    communication.sendToNode();
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}
void vtaskReceiveFromNode(void *pvParameters) {
  while (1) {
    communication.receiveFromNode();
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}
void vtaskSendToSever(void *pvParameters) {
  while (1) {
    communication.sendToSever();
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}
void vtaskProcessWiFi(void *pvParameters) {
  while (1) {
    communication.processWiFi();
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}
void vtaskProcessMQTT(void *pvParameters) {
  while (1) {
    communication.processMQTT();
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}
void vtaskBlocking(void *pvParameters) {
  while (1) {
    // reconnect wifi && mqtt reconnnect
    communication.reconnectWifi();
    communication.reconnectMQTT();
    vTaskDelay(3000 / portTICK_PERIOD_MS);
  }
}