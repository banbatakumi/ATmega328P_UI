#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <Wire.h>

#include "Adafruit_NeoPixel.h"
#include "sound.h"

sound Sound(11);

#define PI 3.1415926535   // 円周率

#define DIN_PIN 6   // D6
#define LED_COUNT 1   // LEDの数

#define SWITCH_LEFT_1_PIN 5   // D5
#define SWITCH_LEFT_2_PIN 4   // D4
#define SWITCH_RIGHT_1_PIN 2   // D2
#define SWITCH_RIGHT_2_PIN 3   // D3
#define BUZZER_PIN 11   // D11

#define SCREEN_WIDTH 128   // OLED 幅指定
#define SCREEN_HEIGHT 64   // OLED 高さ指定
#define OLED_RESET -1   // リセット端子（未使用-1）

Adafruit_NeoPixel pixels(LED_COUNT, DIN_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

bool switch_left_1, switch_left_2, switch_right_1, switch_right_2;

// 受け取るデータ
uint8_t display_mode = 0, set_mode = 0;

int8_t set_value;

int16_t ball_angle, ball_distance;

bool line_check[11];

float voltage_1 = 5.0, voltage_2 = 8.6;

// 送るデータ
uint8_t mode = 0;

uint8_t speed = 50, line_speed = 60;

uint8_t line_reset = 0;

uint8_t dribbler = 0;

void setup() {   // 起動音
      pixels.begin();
      pixels.clear();
      pixels.show();

      tone(BUZZER_PIN, 700, 100);
      delay(300);
      tone(BUZZER_PIN, 500, 80);
      delay(100);
      tone(BUZZER_PIN, 600, 80);
      delay(100);
      tone(BUZZER_PIN, 700, 80);
      delay(100);

      pinMode(SWITCH_LEFT_1_PIN, INPUT);
      pinMode(SWITCH_LEFT_2_PIN, INPUT);
      pinMode(SWITCH_RIGHT_1_PIN, INPUT);
      pinMode(SWITCH_RIGHT_2_PIN, INPUT);

      Serial.begin(28800);

      oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);   // OLED初期設定

      oled.setTextColor(SSD1306_WHITE);

      pixels.setPixelColor(0, pixels.Color(250, 0, 250));
      pixels.show();
}

void loop() {
      // タクトスイッチの状態取得
      switch_left_1 = digitalRead(SWITCH_LEFT_1_PIN);
      switch_left_2 = digitalRead(SWITCH_LEFT_2_PIN);
      switch_right_1 = digitalRead(SWITCH_RIGHT_1_PIN);
      switch_right_2 = digitalRead(SWITCH_RIGHT_2_PIN);

      set_value = 0;
      if (switch_left_1) {
            if (set_mode == 0) display_mode -= 1;
            set_value = -1;
      }
      if (switch_right_1) {
            if (set_mode == 0) display_mode += 1;
            set_value = 1;
      }
      if (switch_left_2) {
            set_mode -= 1;
      }
      if (switch_right_2) {
            set_mode += 1;
      }
      if (switch_left_1 || switch_right_1) {
            tone(BUZZER_PIN, 700, 100);
            delay(150);
      }
      if (switch_left_2 || switch_right_2) {
            tone(BUZZER_PIN, 500, 100);
            delay(150);
      }

      oled.clearDisplay();
      oled.setTextSize(1);

      if (set_mode == 0) {
            if (display_mode != 0) oled.fillTriangle(10, 53, 0, 58, 10, 63, WHITE);
            if (display_mode != 4) oled.fillTriangle(117, 53, 128, 58, 117, 63, WHITE);

            oled.drawFastHLine(0, 10, 128, WHITE);
            oled.setCursor(0, 0);
            oled.print("main");
            oled.setCursor(60, 0);
            oled.print(voltage_1);
            oled.print("v");
            oled.setCursor(96, 0);
            oled.print(voltage_2);
            oled.print("v");
            oled.setTextSize(2);
      }

      Serial.write('a');

      if (display_mode == 0) {
            oled.setTextSize(2);
            if (set_mode == 0) {
                  oled.setCursor(40, 20);
                  oled.print("main");

                  mode = 0;
            } else if (set_mode == 1) {
                  oled.setCursor(22, 20);
                  oled.print("offence");

                  if (abs(set_value) == 1) mode = 1 - mode;
            } else if (set_mode == 2) {
                  oled.setCursor(16, 20);
                  oled.print("diffence");

                  if (abs(set_value) == 1) mode = 2 - mode;
            }

            Serial.write(display_mode);
            Serial.write(mode);

      } else if (display_mode == 1) {
            if (set_mode == 0) {
                  oled.setCursor(16, 20);
                  oled.print("dribbler");

                  dribbler = 0;
            } else if (set_mode == 1) {
                  oled.setCursor(20, 29);
                  oled.print("hold");
                  oled.setCursor(84, 29);
                  oled.print("kick");

                  if (set_value == -1) dribbler = 1;
                  if (set_value == 1) dribbler = 2;
            }

            Serial.write(display_mode);
            Serial.write(dribbler);

      } else if (display_mode == 2) {
            if (set_mode == 0) {
                  oled.setCursor(40, 20);
                  oled.print("ball");
            } else if (set_mode == 1) {
                  oled.setCursor(0, 0);
                  oled.print("ang: ");
                  oled.println(ball_angle);
                  oled.print("dis: ");
                  oled.println(ball_distance);
                  oled.fillCircle(96 + ((100 - ball_distance) / 1.5 + 3) * cos((ball_angle - 90) * PI / 180.000), 32 + ((100 - ball_distance) / 1.5 + 3) * sin((ball_angle - 90) * PI / 180.000), 2, WHITE);
                  oled.drawCircle(96, 32, 28, WHITE);
                  oled.drawCircle(96, 32, 14, WHITE);
                  oled.fillCircle(96, 32, 2, WHITE);
                  oled.drawFastHLine(64, 32, 64, WHITE);
                  oled.drawFastVLine(96, 0, 64, WHITE);
            }

            Serial.write(display_mode);

      } else if (display_mode == 3) {
            if (set_mode == 0) {
                  oled.setCursor(16, 20);
                  oled.print("speed");
            } else if (set_mode == 1) {
                  oled.setCursor(46, 20);
                  oled.print("normal");
                  oled.setCursor(58, 30);
                  oled.print(speed);

                  speed += set_value;
            } else if (set_mode == 1) {
                  oled.setCursor(46, 20);
                  oled.print("normal");
                  oled.setCursor(58, 30);
                  oled.print(line_speed);

                  line_speed += set_value;
            }

            Serial.write(display_mode);
            Serial.write(speed);
            Serial.write(line_speed);

      } else if (display_mode == 4) {
            if (set_mode == 0) {
                  oled.setCursor(40, 20);
                  oled.print("line");
            } else if (set_mode == 1) {
                  if (line_check[0]) {
                        oled.fillRect(63, 13, 4, 9, WHITE);
                  } else {
                        oled.drawRect(63, 13, 4, 9, WHITE);
                  }
                  if (line_check[1]) {
                        oled.fillRect(63, 22, 4, 9, WHITE);
                  } else {
                        oled.drawRect(63, 22, 4, 9, WHITE);
                  }

                  if (line_check[2]) {
                        oled.fillRect(85, 31, 9, 4, WHITE);
                  } else {
                        oled.drawRect(85, 31, 9, 4, WHITE);
                  }
                  if (line_check[3]) {
                        oled.fillRect(76, 31, 9, 4, WHITE);
                  } else {
                        oled.drawRect(76, 31, 9, 4, WHITE);
                  }
                  if (line_check[4]) {
                        oled.fillRect(67, 31, 9, 4, WHITE);
                  } else {
                        oled.drawRect(67, 31, 9, 4, WHITE);
                  }

                  if (line_check[5]) {
                        oled.fillRect(63, 53, 4, 9, WHITE);
                  } else {
                        oled.drawRect(63, 53, 4, 9, WHITE);
                  }
                  if (line_check[6]) {
                        oled.fillRect(63, 44, 4, 9, WHITE);
                  } else {
                        oled.drawRect(63, 44, 4, 9, WHITE);
                  }
                  if (line_check[7]) {
                        oled.fillRect(63, 35, 4, 9, WHITE);
                  } else {
                        oled.drawRect(63, 35, 4, 9, WHITE);
                  }

                  if (line_check[8]) {
                        oled.fillRect(54, 31, 9, 4, WHITE);
                  } else {
                        oled.drawRect(54, 31, 9, 4, WHITE);
                  }
                  if (line_check[9]) {
                        oled.fillRect(45, 31, 9, 4, WHITE);
                  } else {
                        oled.drawRect(45, 31, 9, 4, WHITE);
                  }
                  if (line_check[10]) {
                        oled.fillRect(36, 31, 9, 4, WHITE);
                  } else {
                        oled.drawRect(36, 31, 9, 4, WHITE);
                  }

                  line_reset = abs(set_value);
            }

            Serial.write(display_mode);
            Serial.write(line_reset);
      }
      oled.display();
      Serial.flush();
}