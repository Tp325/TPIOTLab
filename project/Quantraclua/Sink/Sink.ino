#include "Communication.h"
#include "config.h"
Communication communication;
void setup() {
  Serial.begin(9600);
  communication.begin();
  manager.begin();
  //xTaskCreatePinnedToCore(vTaskPrintDebug, "TaskPrintDebug", 1024, NULL, 5, NULL, 1);
  xTaskCreatePinnedToCore(vtaskSendToNode, "taskSendToNode", 6144, NULL, 5, NULL, 1);
  xTaskCreatePinnedToCore(vtaskReceiveFromNode, "taskReceiveFromNode", 6144, NULL, 5, NULL, 1);
  xTaskCreatePinnedToCore(vtaskSendToServer, "taskSendToServer", 4096, NULL, 5, NULL, 1);
  xTaskCreatePinnedToCore(vtaskReceiveFromServer, "taskReceiveFromServer", 4096, NULL, 5, NULL, 1);
  xTaskCreatePinnedToCore(vtaskStorage, "taskStorage", 4096, NULL, 5, NULL, 1);
  xTaskCreatePinnedToCore(vtaskAnalize, "taskAnalize", 4096, NULL, 5, NULL, 1);
  xTaskCreatePinnedToCore(vtaskProcess, "taskProcess", 4096, NULL, 5, NULL, 0);
  xTaskCreatePinnedToCore(vtaskBlocking, "taskBlocking", 4096, NULL, 5, NULL, 0);
  delay(4000);
  vTaskDelete(NULL);
}

void loop() {
}
// // void vTaskPrintDebug(void *pvParameters) {
// //   size_t freeHeap = xPortGetFreeHeapSize();
// //   while (1) {
// //     printf("Free heap size: %u bytes\n", (unsigned int)freeHeap);
// //     vTaskDelay(3000 / portTICK_PERIOD_MS);
// //   }
// // }
void vtaskSendToNode(void *pvParameters) {
  while (1) {
    if (communication.mode == "installation") {
      communication.sendToNode(String(String("{\"SS\":0,\"SID\":") + String(communication.SID) + String("}")));
      vTaskDelay(500 / portTICK_PERIOD_MS);
    }
    communication.sendToNode();
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}
void vtaskStorage(void *pvParameters) {
  uint16_t IDMissingNode = 0;
  while (1) {
    // manager.printNodes();
    IDMissingNode = manager.checkingTimeOut();
    if (IDMissingNode != 0) {
      communication.sendToServer(String(String("{\"SS\":0,\"SID\":") + String(communication.SID) + String(",\"ID\":") + String(IDMissingNode) + String(",\"CM\":\"MS\"}")));
      manager.removeNode(IDMissingNode);
      manager.printNodes();
    }
    vTaskDelay(10000 / portTICK_PERIOD_MS);
  }
}
void vtaskReceiveFromNode(void *pvParameters) {
  while (1) {
    communication.receiveFromNode();
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}
void vtaskReceiveFromServer(void *pvParameters) {
  while (1) {
    communication.receiveFromServer();
    vTaskDelay(20 / portTICK_PERIOD_MS);
  }
}
void vtaskSendToServer(void *pvParameters) {
  while (1) {
    communication.sendToServer();
    vTaskDelay(20 / portTICK_PERIOD_MS);
  }
}
void vtaskAnalize(void *pvParameters) {
  while (1) {
    communication.analizeData();
    vTaskDelay(20 / portTICK_PERIOD_MS);
  }
}
void vtaskProcess(void *pvParameters) {
  while (1) {
    communication.process();
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}
void vtaskBlocking(void *pvParameters) {
  while (1) {
    communication.blocking();
    if (communication.haveToReset == 1)
      ESP.restart();
    vTaskDelay(3000 / portTICK_PERIOD_MS);
  }
}