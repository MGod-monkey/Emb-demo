/*
 * @Author: MGodmonkey
 * @Date: 2021-04-03 20:12:40
 * @LastEditTime: 2021-04-06 21:03:33
 * @LastEditors: MGodmonkey
 * @Description: 交通灯故障监测系统
 * @FilePath: \test_OLED\src\main.cpp
 */

#include <Wire.h>
#include "SSD1306Wire.h"
// 图片库，格式: xmb
#include <OLEDbits.h>
#include <Arduino.h>
#include <PCF8574.h>
#include <Wire.h>

#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>


SSD1306Wire display(0x3c, 22, 23);   // 初识化OLED
PCF8574 LED(0x20);  // 初识化LED

typedef void (*Demo)(void);
#define GREEN_WAIT_TIME 3000  //绿灯通行时长
#define YELLOW_WAIT_TIME 1000 //黄灯等待时长
#define K1 17  //定义K1按键引脚
#define K2 16  //定义K2按键引脚 

int demoMode = 0;
long timer = 0;
int flag = 1;
uint isERROR = 0;
uint8_t LEDCode[] = {0114, 0122, 0141, 0122, 0214, 0122}; // 正确的LED显示编码
uint8_t ERRORCode[] = {0225, 0152, 0261, 017, 0252, 0360};  // 错误的LED显示编码
char *str[] = {"Right to Left", "yellow deng, plase wait!", "Left to Right", "yellow deng, plase wait!", "Down to Left", "yellow deng, plase wait!"};
int delayTime[] = {GREEN_WAIT_TIME, YELLOW_WAIT_TIME, GREEN_WAIT_TIME, YELLOW_WAIT_TIME, GREEN_WAIT_TIME, YELLOW_WAIT_TIME};

// 关于Blynk平台的一些配置
char auth[] = "6LGEA7a3W4_MGKjiuPlyVZyLIiA4OZHD";    //token
char ssid[] = "mi";   //热点名称
char pass[] = "wpq5201314";   //热点密码

// 创建控制台组件
WidgetTerminal terminal(V0);
// 声明函数
void onChange();
void main_run();

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println();
  Serial.printf("now is: %s\n", str[demoMode]);

  // 初识化OLED，pcf8574
  display.init();
  display.flipScreenVertically();
  LED.begin(18, 19, 0);
  LED.write8(LEDCode[demoMode]);
  display.setFont(Syncopate_Bold_9);
  display.drawString(0, 12, "connecting WIFI to");
  display.setFont(ArialMT_Plain_24);
  display.drawString(5, 25, ssid);
  display.display();

  //定义按键中断
  pinMode(K1, INPUT_PULLUP);
  pinMode(K2, INPUT_PULLUP);
  attachInterrupt(K1, onChange, FALLING);

  //物联网模块
  Blynk.begin(auth, ssid, pass);
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
Demo demos[] = {RTL, YELLOW, LTR, YELLOW, CTL, YELLOW};
int demoLength = (sizeof(demos) / sizeof(Demo));

// 显示故障界面
void ERROR() {
  LED.write8(ERRORCode[random(0,6)]);
  drawImage(0, 0, ERROR_width, ERROR_height, ERROR_bits);
  if (flag){
  Serial.println("\n#################################################################");
  Serial.println("警告：正在发生重大电路故障事故，请相关人员立即前往直通520路口进行救援！");
  Serial.println("#################################################################\n");
  terminal.println("###############################################");
  terminal.flush();
  terminal.println("Warning:Something unspeakable is happening!");
  terminal.flush();
  terminal.println("###############################################");
  terminal.flush();
  Blynk.notify("警告:正在发生重大电路故障事故，请相关人员立即前往直通520路口进行救援!");
  flag = 0;
  }
}

// 组件回调函数
BLYNK_CONNECTED() {
  Blynk.syncVirtual(V1);
}
BLYNK_WRITE(V0) {
  // 清除显示屏
  if (String("clear") == param.asStr()) {
    terminal.clear();
  }
  else {
    terminal.println("\nERROR: Unknown command!");
    terminal.flush();
  }
}
BLYNK_WRITE(V1) {
  int buttonState = param.asInt();
  isERROR = buttonState;
}

// 按键中断
void onChange() {
  isERROR = !isERROR;
  while (!digitalRead(K1));
}

// 交通灯主函数
void main_run() {
  display.clear();  //清屏
  if (!isERROR){
    LED.write8(LEDCode[demoMode]);
    demos[demoMode]();
    flag = 1;
    //动画组切换
    if (millis() - timer > delayTime[demoMode]) {
      demoMode = (demoMode + 1)  % demoLength;  
      Serial.printf("now is: %s\n", str[demoMode]);
      terminal.printf("now is: %s\n", str[demoMode]);
      terminal.flush();
      delay(10);
      timer = millis();
    }
  }
  else {
      ERROR();
    }
}


void loop() {
  Blynk.run();
  main_run();
  delay(10);
}
