#include "Arduino.h"
#include "Function.h"

Rs485Sensor::Rs485Sensor() {
}

float Rs485Sensor::getvoltage(int AnalogInput) {
  return AnalogInput / 1024.0 * 5.0;
}

float Rs485Sensor::getSal(int AnalogInput) {
  float Sal = getvoltage(AnalogInput) * slope + intercept;
  Sal = Sal * 0.8063 + 0.1614;
  Sal = Sal * 1.0604 - 1.3692;
  if (getvoltage(AnalogInput) == 0) {
    Sal = 0;
  }
  if (Sal < 0) {
    Sal = 0;
  }
  return Sal;
}

float Rs485Sensor::getTmp(int AnalogInput) {
  float measured_resistance = (resistance * getvoltage(AnalogInput)) / (5 - getvoltage(AnalogInput));
  float temp = 1.0 / (Ko + K1 * logf(1000 * measured_resistance) + K2 * powf(log(1000.0 * measured_resistance), 3)) - 272.15;
  return temp * _slope_temp + _intercept_temp;
}

float Rs485Sensor::getPh(int AnalogInput) {
  return getvoltage(AnalogInput) * slope1 + intercept1;
}

float Rs485Sensor::getNH4(int AnalogInput) {
  float NH4 = exp(((((getvoltage(AnalogInput) - 1.223) / 0.00727) + 168.23) / 23.889) - 7.487);
  NH4 = NH4 * _slope_NH4 + _intercept_NH4;
  NH4 = NH4 * 0.3446 + 0.0766;
  return NH4;
}

float Rs485Sensor::getDo(int AnalogInput) {
  float DO = getvoltage(AnalogInput) * _slope_DO + _intercept_DO;
  DO = DO * 0.582 + 3.2405;  // calibrate DO 3/1/2024
  if (DO < 0) {
    DO = 0;
  }
  return DO;
}


SoftwareSerial Community(16, 17);
TransmitToNode::TransmitToNode() {
}

void TransmitToNode::start() {
  pinMode(32, OUTPUT);
  pinMode(33, OUTPUT);
  Community.begin(9600);
}

DynamicJsonDocument dataForSend(1024);

void TransmitToNode::send(int Id) {
  dataForSend["Id"] = Id;
  String msgToSend;
  serializeJson(dataForSend, msgToSend);
  Community.print(msgToSend);
  delay(200);
}

DynamicJsonDocument DataForReturn(1024);
DynamicJsonDocument Stored(1024);

void TransmitToNode::receive(String DataReceiveFromNode, Rs485Sensor Sensor) {
  deserializeJson(Stored, DataReceiveFromNode);
  fillterData(Sensor);
}

void TransmitToNode::fillterData(Rs485Sensor Sensor) {
  for (int i = 0; i <= 5; i++) {
    this->arrayProcessedData[i] = 0;
  }
  switch (Stored["Id"].as<int>()) {
    case 1:
      DataForReturn["Salanity"] = Sensor.getSal(Stored["Data"].as<int>());
      this->arrayProcessedData[1] = DataForReturn["Salanity"];
      Serial.println("1");
      break;
    case 2:
      DataForReturn["Temperature"] = Sensor.getTmp(Stored["Data"].as<int>());
      this->arrayProcessedData[2] = DataForReturn["Temperature"];
      Serial.println("2");
      break;
    case 3:
      DataForReturn["PH"] = Sensor.getPh(Stored["Data"].as<int>());
      this->arrayProcessedData[3] = DataForReturn["PH"];
      Serial.println("3");
      break;
    case 4:
      DataForReturn["NH4"] = Sensor.getNH4(Stored["Data"].as<int>());
      this->arrayProcessedData[4] =DataForReturn["NH4"]; 
      Serial.println("4");
      break;
    case 5:
      DataForReturn["DO"] = Sensor.getDo(Stored["Data"].as<int>());
      this->arrayProcessedData[5] = DataForReturn["DO"];
      Serial.println("5");
      break;
    default:
      Serial.println("NotFault");
      break;
  }
}


void TransmitToNode::processDataFromNode(Rs485Sensor Sensor) {
  digitalWrite(32, HIGH);
  digitalWrite(33, HIGH);
  delay(1000);
  deserializeJson(DataForReturn, " ");
  for (int i = 0; i <= 5; i++) {
    this->send(i);
    int waitTime = millis();
    bool acceptData = 1;
    while (!Community.available()) {
      if (millis() - waitTime >= 3000) {
        acceptData = 0;
        break;
      } else {
        acceptData = 1;
      }
    }
    if (acceptData == 1) {
      deserializeJson(Stored, "");
      receive(Community.readString(), Sensor);
    }
    delay(1000);
  }
  serializeJson(DataForReturn, processedData);
  Serial.println(processedData);
}


Wifi::Wifi() {
}
void Wifi::start(char *userName, char *password) {
  Serial.print("conecting to ");
  WiFi.begin(userName, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print('.');
  }
  Serial.println("Wifi conected");
}


WiFiClient espClient;
PubSubClient client(espClient);
TransmitToSever::TransmitToSever() {
}
void TransmitToSever::start() {
  client.setServer(this->mqttsever, this->mqttport);
}
void TransmitToSever::reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection..");
    String clientID = "ESP32Client-";
    clientID += String(random(0xffff), HEX);
    if (client.connect(clientID.c_str(), this->mqttuser, this->mqttpass)) {
      Serial.println("connected");
      client.subscribe("dataOfMasterFromAqua");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}
void TransmitToSever::sendDataToSever(String dataForSend, char *topic) {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  client.publish(topic, dataForSend.c_str(), true);
}
Sleep::Sleep() {
}
void Sleep::setup(float minuteToSleep) {
  esp_sleep_enable_timer_wakeup(minuteToSleep * 60 * 1000000);
}
void Sleep::begin() {
  delay(500);
  digitalWrite(32, LOW);
  digitalWrite(33, LOW);
  esp_light_sleep_start();
}