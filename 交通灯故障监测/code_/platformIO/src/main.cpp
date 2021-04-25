/*
 * @Author: your name
 * @Date: 2021-04-03 20:12:40
 * @LastEditTime: 2021-04-06 21:03:33
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \test_OLED\src\main.cpp
 */
#include <Wire.h>               // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Wire.h"        // legacy: #include "SSD1306.h"

// 图片库，格式: xmb
#include <OLEDbits.h>
#include <Arduino.h>
#include <PCF8574.h>
#include <Wire.h>



// Initialize the OLED display using Arduino Wire:
SSD1306Wire display(0x3c, 22, 23);   // ADDRESS, SDA, SCL  -  SDA and SCL usually populate automatically based on your board's pins_arduino.h
PCF8574 LED(0x20);  // add leds to lines      (used as output)

typedef void (*Demo)(void);
#define GREEN_WAIT_TIME 3000  //绿灯通行时长
#define YELLOW_WAIT_TIME 1000 //黄灯等待时长
#define K1 17  //定义K1按键引脚
#define K2 16  //定义K2按键引脚 

int demoMode = 0;
long timer = 0;
int flag = 1;
bool isERROR = false;
uint8_t LEDCode[] = {0114, 0122, 0141, 0122, 0214, 0122};
uint8_t ERRORCode[] = {0225, 0152, 0261, 017, 0252, 0360};
char *string[] = {"Right to Left", "黄灯，请等待！", "Left to Right", "黄灯，请等待！", "Down to Left", "黄灯，请等待！"};
int delayTime[] = {GREEN_WAIT_TIME, YELLOW_WAIT_TIME, GREEN_WAIT_TIME, YELLOW_WAIT_TIME, GREEN_WAIT_TIME, YELLOW_WAIT_TIME};

// 声明函数
void onChange();

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println();
  Serial.printf("now is: %s\n", string[demoMode]);

  // 初识化OLED，pcf8574
  display.init();
  display.flipScreenVertically();
  LED.begin(18, 19, 0);
  LED.write8(LEDCode[demoMode]);

  //定义按键中断
  pinMode(K1, INPUT_PULLUP);
  pinMode(K2, INPUT_PULLUP);
  attachInterrupt(K1, onChange, FALLING);
}

// 图片展示
void drawImage(int16_t int_x, int16_t int_y, int16_t width, int16_t height, const uint8_t *xbm) {
    display.drawXbm(int_x, int_y, width, height, xbm);
    display.display();
    delay(83);
    display.clear();
}

// Center To Left：动画组
void CTL() {
  drawImage(0, 0, CTL_width, CTL_height, CTL_1_bits);
  drawImage(0, 0, CTL_width, CTL_height, CTL_2_bits);
  drawImage(0, 0, CTL_width, CTL_height, CTL_3_bits);
  drawImage(0, 0, CTL_width, CTL_height, CTL_4_bits);
  drawImage(0, 0, CTL_width, CTL_height, CTL_5_bits);
  drawImage(0, 0, CTL_width, CTL_height, CTL_6_bits);
}

// Left To Right: 动画组
void LTR() {
  drawImage(0, 0, LTR_width, LTR_height, LTR_1_bits);
  drawImage(0, 0, LTR_width, LTR_height, LTR_2_bits);
  drawImage(0, 0, LTR_width, LTR_height, LTR_3_bits);
  drawImage(0, 0, LTR_width, LTR_height, LTR_4_bits);
  drawImage(0, 0, LTR_width, LTR_height, LTR_5_bits);
  drawImage(0, 0, LTR_width, LTR_height, LTR_6_bits);
}

// Right To Left: 动画组
void RTL() {
  drawImage(0, 0, RTL_width, RTL_height, RTL_1_bits);
  drawImage(0, 0, RTL_width, RTL_height, RTL_2_bits);
  drawImage(0, 0, RTL_width, RTL_height, RTL_3_bits);
  drawImage(0, 0, RTL_width, RTL_height, RTL_4_bits);
  drawImage(0, 0, RTL_width, RTL_height, RTL_5_bits);
  drawImage(0, 0, RTL_width, RTL_height, RTL_6_bits);
}

// Waiting: 动画组
void YELLOW() {
  drawImage(0, 0, YELLOW_width, YELLOW_height, YELLOW_bits);
  drawImage(0, 0, YELLOW_width, YELLOW_height, YELLOW_bits);
  drawImage(0, 0, YELLOW_width, YELLOW_height, YELLOW_bits);
  drawImage(0, 0, YELLOW_width, YELLOW_height, YELLOW_bits);
  drawImage(0, 0, YELLOW_width, YELLOW_height, YELLOW_bits);
  drawImage(0, 0, YELLOW_width, YELLOW_height, YELLOW_bits);
}

// 显示故障界面
void ERROR() {
  LED.write8(ERRORCode[random(0,6)]);
  drawImage(0, 0, ERROR_width, ERROR_height, ERROR_bits);
  if (flag){
  Serial.println("\n#################################################################");
  Serial.println("警告：正在发生重大电路故障事故，请相关人员立即前往直通520路口进行救援！");
  Serial.println("#################################################################\n");
  flag = 0;
  }
}


Demo demos[] = {RTL, YELLOW, LTR, YELLOW, CTL, YELLOW};
int demoLength = (sizeof(demos) / sizeof(Demo));
// 按键中断
void onChange() {
  isERROR = !isERROR;
  while (!digitalRead(K1));
}

void loop() {
    display.clear();  //清屏
  if (!isERROR){
    LED.write8(LEDCode[demoMode]);
    demos[demoMode]();
    flag = 1;
    // draw the current demo method
    if (millis() - timer > delayTime[demoMode]) {
      demoMode = (demoMode + 1)  % demoLength;  //动画组切换
      Serial.printf("now is: %s\n", string[demoMode]);
      timer = millis();
    }
  }
  else {
      ERROR();
    }
  delay(10);
}
