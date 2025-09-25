#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include "uRTCLib.h"

// Ngưỡng lọc err
#define OffsetErr 100
// Định nghĩa chân CS lora
#define LORA_CS 10
// Định nghĩa chân RST lora
#define LORA_RST 48
// Định nghĩa chân DIO lora
#define LORA_DI0 45
// Định nghĩa chân ADC đọc điện áp cấp vào board
#define BAT_SENSE_12V 16
// Định nghĩa chân ADC đọc điện áp pin
#define BAT_SENSE_3V3 3
// Định nghĩa chân chốt
#define SR_LATCH_PIN 21
// Định nghĩa chân Clock
#define SR_SCK_PIN 12
// Định nghĩa chân MOSI
#define SR_MOSI_PIN 11
// Định nghĩa chân Reset
#define SR_MR_PIN 47
// Định nghĩa chân Enable
#define SR_OE_PIN 14
// Định nghĩa chân MISO
#define SR_MISO_PIN 13
// Định nghĩa trạng thái
#define output_is_enable 0
// Định nghĩa trạng thái
#define output_is_disable 1
// Định nghĩa trạng thái
#define register_reset 0
// Định nghĩa trạng thái
#define register_enable 1

uint8_t day = 0;
uint8_t date = 0;
uint8_t month = 0;
uint8_t year = 0;
uint8_t hour = 0;
uint8_t minute = 0;
uint8_t second = 0;
uRTCLib rtc(0x68);
LiquidCrystal_I2C lcd(0x27, 20, 4);

float readVotage(int pinread)
{
  uint16_t Val = 0;
  uint16_t ValRef = 0;
  byte Count = 0;
  float Vs = 0;
  float CC = 0;
  if (pinread == BAT_SENSE_12V)
  {
    ValRef = analogRead(pinread);
    while (ValRef < 2975)
    {
      ValRef = analogRead(pinread);
    }

    for (int i = 0; i < 30000; i++)
    {
      Val = analogRead(pinread);
      if ((Val < (ValRef + OffsetErr)) && (Val > (ValRef - OffsetErr)))
      {
        if (Val < 3117)
          Vs = map(Val, 3117, 2975, 1152, 1105) / 100.0;
        else if (Val < 3276)
          Vs = map(Val, 3276, 3117, 1200, 1152) / 100.0;
        else if (Val < 3452)
          Vs = map(Val, 3452, 3276, 1250, 1200) / 100.0;
        else if (Val < 3654)
          Vs = map(Val, 3652, 3452, 1300, 1250) / 100.0;
        else if (Val < 3869)
          Vs = map(Val, 3869, 3654, 1350, 1300) / 100.0;
        else if (Val < 4096)
          Vs = map(Val, 4096, 3854, 1400, 1350) / 100.0;
        CC += Vs;
        Count++;
        if (Count == 5)
          break;
      }
    }
  }
  else
  {
    ;
    /* Code cho hàm đọc điện áp Pin
                R31              R33
               ------          ------
     V_BAT -- | 6.8K |--------|  10K |------ GND
               ------     |    ------
                          |
                         ADC                                         */
  }
  return CC / 5.0;
}

// Viết về dạng OOP để dùng chung với thư viện khác trên cùng bus SPI
void setbit(uint8_t _data_)
{
  SPI.beginTransaction(SPISettings(200000, MSBFIRST, SPI_MODE0));
  digitalWrite(SR_LATCH_PIN, 0);
  SPI.transfer(_data_);
  digitalWrite(SR_LATCH_PIN, 1);
  SPI.endTransaction();
}

// Viết về dạng OOP để dùng chung với thư viện khác
void write2byte(uint8_t _data_, uint8_t _data2_)
{
  SPI.beginTransaction(SPISettings(200000, MSBFIRST, SPI_MODE0));
  digitalWrite(SR_LATCH_PIN, 0);
  SPI.transfer(_data_);
  SPI.transfer(_data2_);
  digitalWrite(SR_LATCH_PIN, 1);
  SPI.endTransaction();
}

void setup()
{
  // Khởi tạo bộ mở rộng chân
  // Viết về dạng OOP để dùng chung với thư viện khác
  pinMode(SR_MR_PIN, OUTPUT);
  pinMode(SR_OE_PIN, OUTPUT);
  pinMode(SR_LATCH_PIN, OUTPUT);
  digitalWrite(SR_MR_PIN, register_enable);
  digitalWrite(SR_OE_PIN, output_is_enable);
  SPI.begin();
  SPI.beginTransaction(SPISettings(200000, MSBFIRST, SPI_MODE0));
  digitalWrite(SR_LATCH_PIN, 0);
  SPI.transfer(0x00);
  digitalWrite(SR_LATCH_PIN, 1);
  SPI.endTransaction();

  // khởi tạo LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);

  for (int j = 0; j < 2; j++)
  {
    for (int i = 0; i < 6; i++)
    {
      setbit(0x40 >> i);
      delay(100);
    }
    for (int i = 0; i < 6; i++)
    {
      setbit(0x02 << i);
      delay(100);
    }
  }
  setbit(0x00);
  delay(500);
}

void loop()
{
  // đọc thời gian RTC DS3231
  rtc.refresh();
  day = rtc.dayOfWeek();
  date = rtc.day();
  month = rtc.month();
  year = rtc.year();
  hour = rtc.hour();
  minute = rtc.minute();
  second = rtc.second();

  lcd.setCursor(0, 0);
  lcd.print("T");
  lcd.print(day);
  lcd.print(" ");

  lcd.print(date);
  lcd.print("/");
  lcd.print(month);
  lcd.print("/");
  lcd.print(year);

  lcd.print(" ");
  lcd.print(hour);
  lcd.print(":");
  lcd.print(minute);
  lcd.print(":");
  lcd.print(second);
  // Đọc điện áp cấp vào 10-14V
  float votage = readVotage(BAT_SENSE_12V);
  lcd.setCursor(0, 3);
  lcd.print("Votage 1: ");
  lcd.print(votage, 1);
  lcd.print("V");
}