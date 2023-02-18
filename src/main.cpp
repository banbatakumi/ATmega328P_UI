#include <Arduino.h>

#include "Adafruit_NeoPixel.h"
#include "sound.h"

sound Sound(11);

#define DIN_PIN 6   // D6
#define LED_COUNT 1   // LEDの数
#define SWITCH_LEFT_1_PIN 5   // D5
#define SWITCH_LEFT_2_PIN 4   // D4
#define SWITCH_RIGHT_1_PIN 2   // D2
#define SWITCH_RIGHT_2_PIN 3   // D3
#define BUZZER_PIN 11   // D11

bool switch_left_1, switch_left_2, switch_right_1, switch_right_2;

Adafruit_NeoPixel pixels(LED_COUNT, DIN_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
      pinMode(SWITCH_LEFT_1_PIN, INPUT);
      pinMode(SWITCH_LEFT_2_PIN, INPUT);
      pinMode(SWITCH_RIGHT_1_PIN, INPUT);
      pinMode(SWITCH_RIGHT_2_PIN, INPUT);

      Serial.begin(9600);

      pixels.begin();
      pixels.clear();
      pixels.show();

      // 起動音
      tone(BUZZER_PIN, 700, 100);
      delay(300);
      tone(BUZZER_PIN, 500, 80);
      delay(100);
      tone(BUZZER_PIN, 600, 80);
      delay(100);
      tone(BUZZER_PIN, 700, 80);
      delay(100);

      pixels.setPixelColor(0, pixels.Color(250, 0, 250));
      pixels.show();
}

void loop() {
      // タクトスイッチの状態取得
      switch_left_1 = digitalRead(SWITCH_LEFT_1_PIN);
      switch_left_2 = digitalRead(SWITCH_LEFT_2_PIN);
      switch_right_1 = digitalRead(SWITCH_RIGHT_1_PIN);
      switch_right_2 = digitalRead(SWITCH_RIGHT_2_PIN);
      int value;
      value = Serial.read();
      Serial.println(value);
}