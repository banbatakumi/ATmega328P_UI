#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <Wire.h>

#include "Adafruit_NeoPixel.h"
#include "sound.h"

sound Sound(11);

#define PI 3.1415926535   // 円周率

#define DIN_PIN 6   // D6
#define LED_COUNT 2   // LEDの数

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

void display_mode_main();
void display_mode_ball();
void display_mode_line();
void display_mode_speed();
void display_mode_dribbler();
void display_mode_imu();
void display_mode_ball_catch();
void display_mode_cam();

bool switch_left_1, switch_left_2, switch_right_1, switch_right_2;

int8_t display_mode = 0, set_mode = 0;

int8_t set_value = 0;

// 受け取るデータ
bool line_check[11];

uint8_t ball_catch_left, ball_catch_right, ball_catch_all;

int16_t goal_angle, goal_wide;

int16_t yaw;

int16_t ball_angle, ball_distance;

// 送るデータ
uint8_t mode = 0;

uint8_t move_speed = 60, line_move_speed = 70;

uint8_t goal_angle_mode = 2;

uint8_t line_reset = 0;

int16_t set_yaw = 0;

uint8_t dribbler = 0;

void setup() {   // 起動音
      pixels.begin();
      pixels.setPixelColor(0, pixels.Color(250, 0, 0));
      pixels.setPixelColor(1, pixels.Color(250, 0, 0));
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

      Serial.begin(38400);

      oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);   // OLED初期設定

      oled.setTextColor(SSD1306_WHITE);

      display_mode_main();
      display_mode_ball();
      display_mode_line();
      display_mode_speed();
      display_mode_dribbler();
      display_mode_imu();
      display_mode_ball_catch();
      display_mode_cam();
}

void loop() {
      // タクトスイッチの状態取得
      switch_left_1 = digitalRead(SWITCH_LEFT_1_PIN);
      switch_left_2 = digitalRead(SWITCH_LEFT_2_PIN);
      switch_right_1 = digitalRead(SWITCH_RIGHT_1_PIN);
      switch_right_2 = digitalRead(SWITCH_RIGHT_2_PIN);

      set_value = 0;
      if (switch_left_1) {
            if (set_mode == 0 && display_mode > 0) display_mode -= 1;
            set_value = -1;
      }
      if (switch_right_1) {
            if (set_mode == 0 && display_mode < 7) display_mode += 1;
            set_value = 1;
      }
      if (switch_left_2 && mode == 0) set_mode -= 1;
      if (switch_right_2 && mode == 0) set_mode += 1;

      if (switch_left_1 || switch_right_1) {
            tone(BUZZER_PIN, 750, 90);
            delay(100);
      }
      if (switch_left_2 || switch_right_2) {
            tone(BUZZER_PIN, 650, 90);
            delay(100);
      }

      oled.clearDisplay();
      oled.setTextSize(1);

      pixels.clear();
      if (set_mode == 0) {
            pixels.setPixelColor(0, pixels.Color(0, 250, 0));
            pixels.setPixelColor(1, pixels.Color(0, 250, 0));

            if (display_mode > 0) oled.fillTriangle(10, 53, 0, 58, 10, 63, WHITE);
            if (display_mode < 7) oled.fillTriangle(117, 53, 128, 58, 117, 63, WHITE);
      }

      oled.setTextSize(2);
      if (display_mode == 0) {
            display_mode_main();
      } else if (display_mode == 1) {
            display_mode_imu();
      } else if (display_mode == 2) {
            display_mode_speed();
      } else if (display_mode == 3) {
            display_mode_line();
      } else if (display_mode == 4) {
            display_mode_ball();
      } else if (display_mode == 5) {
            display_mode_dribbler();
      } else if (display_mode == 6) {
            display_mode_ball_catch();
      } else if (display_mode == 7) {
            display_mode_cam();
      }

      pixels.show();

      oled.display();
      Serial.flush();
}

void display_mode_main() {
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
      } else if (set_mode == 3) {
            oled.setCursor(16, 20);
            oled.println("PID test");

            if (abs(set_value) == 1) mode = 3 - mode;
      } else if (set_mode == 4) {
            oled.setCursor(8, 20);
            oled.print("ball test");

            if (abs(set_value) == 1) mode = 4 - mode;
      } else if (set_mode == 5) {
            set_mode = 4;
      } else if (set_mode == -1) {
            set_mode = 0;
      }

      if (mode != 0) {
            oled.setTextSize(1);
            oled.setCursor(34, 40);
            oled.print("moving now");
      }

      if (abs(set_value) == 1 || set_mode == 0) {
            Serial.write('a');
            Serial.write(display_mode);
            Serial.write(mode);
            Serial.write(move_speed);
            Serial.write(line_move_speed);
            Serial.write(goal_angle_mode);
      }
}

void display_mode_dribbler() {
      if (set_mode == 0) {
            oled.setCursor(16, 20);
            oled.print("dribbler");

            dribbler = 0;
      } else if (set_mode == 1) {
            oled.setCursor(10, 20);
            oled.print("hold");
            oled.setCursor(70, 20);
            oled.print("kick");

            if (set_value == -1) dribbler = 1;
            if (set_value == 1) dribbler = 2;
      } else if (set_mode == 2) {
            set_mode = 1;
      } else if (set_mode == -1) {
            set_mode = 0;
      }

      Serial.write('a');
      Serial.write(display_mode);
      Serial.write(dribbler);
}

void display_mode_speed() {
      if (set_mode == 0) {
            oled.setCursor(34, 20);
            oled.print("speed");
      } else if (set_mode == 1) {
            oled.setCursor(34, 15);
            oled.print("speed");
            oled.setCursor(46, 40);
            oled.print(move_speed);
            oled.print("%");

            move_speed += set_value * 5;
      } else if (set_mode == 2) {
            oled.setCursor(4, 15);
            oled.print("line speed");
            oled.setCursor(46, 40);
            oled.print(line_move_speed);
            oled.print("%");

            line_move_speed += set_value * 5;
      } else if (set_mode == 3) {
            set_mode = 2;
      } else if (set_mode == -1) {
            set_mode = 0;
      }

      Serial.write('a');
      Serial.write(display_mode);
      Serial.write(move_speed);
      Serial.write(line_move_speed);
}

void display_mode_ball() {
      if (Serial.read() == 'a') {
            uint8_t ball_angle_plus, ball_angle_minus;

            ball_angle_plus = Serial.read();
            ball_angle_minus = Serial.read();
            ball_angle = ball_angle_plus == 0 ? ball_angle_minus * -1 : ball_angle_plus;
            ball_angle -= ball_angle > 180 ? 360 : (ball_angle < -180 ? -360 : 0);
            ball_distance = Serial.read();
      }

      if (set_mode == 0) {
            oled.setCursor(40, 20);
            oled.print("ball");
      } else if (set_mode == 1) {
            oled.setTextSize(1);
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
      } else if (set_mode == 2) {
            set_mode = 1;
      } else if (set_mode == -1) {
            set_mode = 0;
      }

      Serial.write('a');
      Serial.write(display_mode);
}

void display_mode_line() {
      if (Serial.read() == 'a') {
            for (uint8_t count = 0; count <= 10; count++) line_check[count] = Serial.read();
      }

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
                  oled.fillRect(36, 31, 9, 4, WHITE);
            } else {
                  oled.drawRect(36, 31, 9, 4, WHITE);
            }
            if (line_check[9]) {
                  oled.fillRect(45, 31, 9, 4, WHITE);
            } else {
                  oled.drawRect(45, 31, 9, 4, WHITE);
            }
            if (line_check[10]) {
                  oled.fillRect(54, 31, 9, 4, WHITE);
            } else {
                  oled.drawRect(54, 31, 9, 4, WHITE);
            }

            line_reset = abs(set_value);
      } else if (set_mode == 2) {
            set_mode = 1;
      } else if (set_mode == -1) {
            set_mode = 0;
      }

      Serial.write('a');
      Serial.write(display_mode);
      Serial.write(line_reset);
}

void display_mode_imu() {
      if (Serial.read() == 'a') {
            uint8_t yaw_plus, yaw_minus;
            yaw_plus = Serial.read();
            yaw_minus = Serial.read();
            yaw = yaw_plus == 0 ? yaw_minus * -1 : yaw_plus;
      }

      if (set_mode == 0) {
            oled.setCursor(46, 20);
            oled.print("imu");
      } else if (set_mode == 1) {
            oled.setCursor(10, 10);
            oled.print("yaw");
            oled.setCursor(10, 30);
            oled.print(yaw);
            oled.drawCircle(96, 32, 29, WHITE);
            oled.fillCircle(96 + 29 * cos((yaw * -1 - 90) * PI / 180.000), 32 + 29 * sin((yaw * -1 - 90) * PI / 180.000), 3, WHITE);
            oled.drawLine(96, 32, 96 + 29 * cos((yaw * -1 - 90) * PI / 180.000), 32 + 29 * sin((yaw * -1 - 90) * PI / 180.000), WHITE);

            set_yaw = abs(set_value);
      } else if (set_mode == 2) {
            set_mode = 1;
      } else if (set_mode == -1) {
            set_mode = 0;
      }

      Serial.write('a');
      Serial.write(display_mode);
      Serial.write(set_yaw);
}

void display_mode_ball_catch() {
      if (Serial.read() == 'a') {
            ball_catch_all = Serial.read();
            ball_catch_left = Serial.read();
            ball_catch_right = Serial.read();
      }

      if (set_mode == 0) {
            oled.setCursor(4, 20);
            oled.print("ball catch");
      } else if (set_mode == 1) {
            oled.setCursor(0, 0);
            oled.print("both: ");
            oled.println(ball_catch_all);
            oled.print("left: ");
            oled.println(ball_catch_left);
            oled.print("right: ");
            oled.print(ball_catch_right);
      } else if (set_mode == 2) {
            set_mode = 1;
      } else if (set_mode == -1) {
            set_mode = 0;
      }

      Serial.write('a');
      Serial.write(display_mode);
}

void display_mode_cam() {
      if (Serial.read() == 'a') {
            goal_angle = Serial.read() - 106;
            goal_wide = Serial.read();
      }

      if (set_mode == 0) {
            oled.setCursor(46, 20);
            oled.print("cam");
      } else if (set_mode == 1) {
            oled.setCursor(0, 0);
            oled.print("goal angle mode: ");
            oled.println(goal_angle_mode);
            oled.print("goal angle: ");
            oled.println(goal_angle);
            oled.print("goal wide: ");
            oled.println(goal_wide);
            if (set_value == -1) goal_angle_mode = 1;
            if (set_value == 1) goal_angle_mode = 2;
      } else if (set_mode == 2) {
            oled.setCursor(28, 20);
            oled.print("cam of");
            if (abs(set_value) == 1) goal_angle_mode = 0;
      } else if (set_mode == 3) {
            set_mode = 2;
      } else if (set_mode == -1) {
            set_mode = 0;
      }

      Serial.write('a');
      Serial.write(display_mode);
      Serial.write(goal_angle_mode);
}