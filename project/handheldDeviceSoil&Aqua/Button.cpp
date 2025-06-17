#include "Button.h"
Button::Button(int down, int up, int right, int left, int ok, int send, int back) {
  this->down = down;
  this->up = up;
  this->right = right;
  this->left = left;
  this->ok = ok;
  this->send = send;
  this->back = back;
}
void Button::begin() {
  pinMode(this->down, INPUT_PULLUP);
  pinMode(this->up, INPUT_PULLUP);
  pinMode(this->right, INPUT_PULLUP);
  pinMode(this->left, INPUT_PULLUP);
  pinMode(this->ok, INPUT_PULLUP);
  pinMode(this->send, INPUT_PULLUP);
  pinMode(this->back, INPUT_PULLUP);
  // pinMode(this->down, INPUT);
  // pinMode(this->up, INPUT);
  // pinMode(this->right, INPUT);
  // pinMode(this->left, INPUT);
  // pinMode(this->ok, INPUT);
  // pinMode(this->send, INPUT);
  // pinMode(this->back, INPUT);
}
void Button::check() {
  // Serial.printf("down:%d , up :%d,right:%d , left:%d, back:%d , ok :%d, send:%d\n", digitalRead(down), digitalRead(up), digitalRead(right), digitalRead(left), digitalRead(ok), digitalRead(back), digitalRead(send));
  if (digitalRead(down) == 0 || digitalRead(up) == 0 || digitalRead(right) == 0 || digitalRead(left) == 0 || digitalRead(ok) == 0 || digitalRead(back) == 0 || digitalRead(send) == 0) {
    isButtonPress = 1;
    isReadyToSleep = 0;
  }
  if (currentPage == 0 || currentPage == 3 || currentPage == 7) {
    if (digitalRead(down) == 0) {
      y = 3;
    }
    if (digitalRead(up) == 0) {
      y = 1;
    }
    if (digitalRead(right) == 0) {
      x = 12;
    }
    if (digitalRead(left) == 0) {
      x = 0;
    }
    if (digitalRead(ok) == 0) {
      ///////
      switch (currentPage) {
        case 0:
          isSellectPress = 1;
          if (x == 0 && y == 1)
            currentPage = 1;
          if (x == 12 && y == 1)
            currentPage = 2;
          if (x == 0 && y == 3) {
            currentPage = 3;
            x = 0;
            y = 1;
          }
          if (x == 12 && y == 3) {
            currentPage = 4;
          }
          vTaskDelay(250 / portTICK_PERIOD_MS);
          break;
        case 3:
          isSellectPress = 1;
          if (x == 0 && y == 1) {
            currentPage = 5;
          }
          if (x == 12 && y == 1) {
            currentPage = 6;
          }
          if (x == 0 && y == 3) {
            currentPage = 7;
            x = 0;
            y = 1;
          }
          if (x == 12 && y == 3) {
          }
          vTaskDelay(250 / portTICK_PERIOD_MS);
          break;
        case 7:
          //////////
          if (currentPage == 7) {
            isSellectPress = 1;
            if (x == 0 && y == 1) {
              deviceMode = 1;
            }
            if (x == 12 && y == 1) {
              deviceMode = 2;
            }
            if (x == 0 && y == 3) {
            }
            if (x == 12 && y == 3) {
            }
          }
          vTaskDelay(250 / portTICK_PERIOD_MS);
          break;
      }
    }
  }
  // Serial.printf("x :%d, y:%d\n", x, y);
  // Serial.println(currentPage);
  if (currentPage == 2) {
    if (digitalRead(send) == 0) {
      isSendPress = 1;
      vTaskDelay(250 / portTICK_PERIOD_MS);
    }
  }
  if (digitalRead(back) == 0 && currentPage != 0) {
    currentPage = 0;
    x = 0;
    y = 1;
    vTaskDelay(250 / portTICK_PERIOD_MS);
  }
}