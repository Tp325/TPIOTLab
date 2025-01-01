#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
#include <PubSubClient.h>
#include <WiFi.h>
#include "DHT.h"

int quat = 25;
int dem = 0;
int demsim = 0;
int ledblue = 12;
int ledyl = 14;
int count = 0;
unsigned long int thoigian = 0;
String msg;
// RX 16
// TX 17
// khởi tạo  SoftwareSerial
SoftwareSerial myserial(16, 17);
// cấu hình wifi
const char* ssid = "Phong_4";
const char* pass = "1234512345";

// Cấu hình MQTT Server
const char* mqttsever = "103.221.220.183";
const int mqttport = 1883;
const char* mqttuser = "api1@Iotlab";
const char* mqttpass = "Iotlab@2023";
WiFiClient espClient;
PubSubClient client(espClient);

// cấu hình json
DynamicJsonDocument doc(1024);

// DHT
#define DHTPIN 26
#define DHTTYPE DHT22
DHT dht(26, DHT22);


// cài đặt wifi
void setup_wifi() {
  Serial.print("conecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(ledyl, HIGH);
    delay(500);
    Serial.print('.');
  }
  Serial.println("");
  digitalWrite(ledyl, LOW);
  Serial.println("Wifi conected");
}

// kết nối lại mqtt server
void reconnectmqtt() {
  while (!client.connected()) {
    digitalWrite(ledyl, LOW);
    Serial.print("Attempting MQTT connection..");
    String clientID = "ESP32Client-";
    clientID += String(random(0xffff), HEX);
    if (client.connect(clientID.c_str(), mqttuser, mqttpass)) {
      Serial.println("connected");
      client.subscribe("Alert_Service");
    } else {
      digitalWrite(ledyl, HIGH);
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 2 seconds");
      delay(2000);
    }
  }
}

void failsim() {
  thoigian = millis();
  String check = "";
  while (myserial.read() != 65) {
    if (millis() - thoigian >= 10000) {
      check = "ERORR";
      Serial.println(check);
      break;
    }
  }
  if (check != "ERORR") {
    Serial.println(dem);
    Serial.println("OK");
    dem++;
    delay(1000);
  }
}
// nhận message từ mqtt server
void callbackmqtt(char* topic, byte* message, unsigned int length) {
  for (int i = 0; i < length; i++) {
    msg += (char)message[i];
  }
}

// gọi số điện thoại
void callsim(String number) {
  switch (dem) {
    case 0:
      myserial.println("AT");
      delay(500);
      failsim();
      break;
    case 1:
      myserial.println("AT+CREG?");
      delay(500);
      failsim();
      break;
    case 2:
      myserial.println("AT+CGREG?");
      delay(500);
      failsim();
      break;
    case 3:
      Serial.println(number);
      myserial.println("ATD0" + number + ";<CR><LF>");
      failsim();
      digitalWrite(ledblue, HIGH);
      delay(10000);
      digitalWrite(ledblue, LOW);
      dem = 0;
      break;
  }
}

// gửi tin nhắn
void sendsim(String number, String send) {
  switch (dem) {
    case 0:
      myserial.println("AT<CR><LF>");
      delay(500);
      failsim();
      break;
    case 1:
      myserial.println("AT+CMGF=1<CR><LF>");
      delay(500);
      failsim();
      break;
    case 2:
      myserial.println("AT+CMGS=\"0"+number+"\"\r");
      Serial.println(send + "\t" + number);
      delay(500);
      myserial.println(send);
      myserial.println(char(26));
      failsim();
      digitalWrite(ledblue, HIGH);
      delay(2000);
      digitalWrite(ledblue, LOW);
      msg = "";
      dem = 0;
      break;
  }
}

void setup() {
  Serial.begin(9600);
  myserial.begin(9600);                   // khởi tạo softwareSerial
  setup_wifi();                           // khởi tạo wifi
  client.setServer(mqttsever, mqttport);  // khởi tạo thông tin server
  client.setCallback(callbackmqtt);       // nhận tin nhắn từ server
  pinMode(ledblue, OUTPUT);               // khởi tạo đèn xác nhận
  pinMode(ledyl, OUTPUT);                 // khởi tạo đèn tín hiệu chờ
  pinMode(quat, OUTPUT);                  // quat
  dht.begin();                            // dht begin
}

void loop() {
  if (!client.connected()) {
    reconnectmqtt();
  }
  client.loop();
  deserializeJson(doc, msg);  // thêm msg vào json doc
  if (dht.readTemperature() >= 37) {
    digitalWrite(quat, HIGH);
  } else {
    digitalWrite(quat, LOW);
  }
  if (msg != "") {
    callsim(doc["call"].as<String>());                               // gọi
    sendsim(doc["call"].as<String>(), doc["message"].as<String>());  // nhắn tin
  }
}
