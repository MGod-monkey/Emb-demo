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

int demoMode = 0, badLED = -1, flag = 1, m = 0;
long timer = 0;
bool isERROR = false;
uint isERROR_1 = 0;
uint isERROR_2 = 0;
uint OFFLED = 0b11111111;
uint8_t LEDCode[] = {0114, 0122, 0141, 0122, 0214, 0122}; // 正确的LED显示编码
uint8_t ERRORCode[] = {0225, 0152, 0261, 017, 0252, 0360, 0340, 07, 070};  // 错误的LED显示编码
char *LEDstr[] = {"RL", "YL", "GL", "RR", "YR", "GR", "RC", "GC"};
char *str[] = {"Right to Left", "yellow deng, plase wait!", "Left to Right", "yellow deng, plase wait!", "Down to Left", "yellow deng, plase wait!"};
int delayTime[] = {GREEN_WAIT_TIME, YELLOW_WAIT_TIME, GREEN_WAIT_TIME, YELLOW_WAIT_TIME, GREEN_WAIT_TIME, YELLOW_WAIT_TIME};

// 关于Blynk平台的一些配置
char auth[] = "BHtYAYW7Pqw99dOyg2XIaV9OLzCRKBbw";    //token
char ssid[] = "mi";   //热点名称
char pass[] = "wpq5201314";   //热点密码

// 创建控制台组件
WidgetTerminal terminal(V0);
// 声明函数
void ERROR_1();
void ERROR_2();
void main_run();

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println();
  // Serial.printf("now is: %s\n", str[demoMode]);

  // 初识化OLED，pcf8574
  display.init();
  display.flipScreenVertically();
  LED.begin(22, 23, 0);
  // LED.write8(LEDCode[demoMode]);
  display.setFont(Syncopate_Bold_9);
  display.drawString(0, 12, "connecting WIFI to");
  display.setFont(ArialMT_Plain_24);
  display.drawString(5, 25, ssid);
  display.display();

  //定义按键中断
  pinMode(K1, INPUT_PULLUP);
  pinMode(K2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(K1), ERROR_1, FALLING);
  attachInterrupt(digitalPinToInterrupt(K2), ERROR_2, FALLING);

  //物联网模块
  Blynk.begin(auth, ssid, pass);
  randomSeed(115200);
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
  drawImage(0, 0, ERROR_width, ERROR_height, ERROR_bits);
  if (flag && isERROR_2){
    Serial.println("\n#################################################################");
    Serial.println("警告：正在发生重大电路故障事故，请相关人员立即前往直通520路口进行救援！");
    Serial.println("#################################################################");
    Serial.printf("提示：监测到LED{%s}疑似损坏！\n", LEDstr[badLED]);
    Serial.println("#################################################################\n");
    terminal.println("###############################################");
    terminal.flush();
    terminal.println("Warning:Something unspeakable is happening!");
    terminal.flush();
    terminal.printf("Suspected damage to LED{%s} was detected\n", LEDstr[badLED]);
    terminal.flush();
    terminal.println("###############################################");
    terminal.flush();
    Blynk.notify("警告:正在发生重大电路故障事故，请相关人员立即前往直通520路口进行救援!");
  flag = 0;
  }
  else if(flag && isERROR_1){
    Serial.println("\n#################################################################");
    Serial.println("警告：正在发生重大电路故障事故，请相关人员立即前往直通520路口进行救援！");
    Serial.println("#################################################################");
    Serial.printf("提示：此次故障疑似神秘能量操控！");
    Serial.println("\n#################################################################\n");
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
  isERROR_1 = param.asInt();
  if (isERROR_1){
    m = random(0,9);  //从0-8随机生成一个数
    LED.write8(ERRORCode[m]);
    isERROR = true;
  }
  else  
  {
    isERROR = false;
  }
}
BLYNK_WRITE(V2) {
  isERROR_2 = param.asInt();
  if (isERROR_2){
  int n = random(0,8);
  OFFLED = ~(0b00000001 << n);
  }
  else{
    OFFLED = 0b11111111;
    isERROR = false;
  }
}

// 按键中断
// 错误模式1：随机亮3盏灯
void ERROR_1() {
  isERROR_1 = !isERROR_1;
  if (isERROR_1){
    m = random(0,9);  //从0-8随机生成一个数
    isERROR = true;
  }
  else   isERROR = false;
  while (!digitalRead(K1));
}
// 错误模式2：模拟现实，随机烂掉一盏灯，即8盏灯中，有一盏灯一直不亮
void ERROR_2() {
  isERROR_2 = !isERROR_2;
  if (isERROR_2){
  int n = random(0,8);
  OFFLED = ~(0b00000001 << n);
  }
  else{
    OFFLED = 0b11111111;
    isERROR = false;
  }
  while (!digitalRead(K2));
}

// 扫描函数
int scan() {
  badLED = -1;
  if ((LEDCode[demoMode] & OFFLED) != LEDCode[demoMode]){
    int n = (LEDCode[demoMode] & OFFLED) ^ LEDCode[demoMode];
    while (n>=1){
      badLED+=1;
      n /= 2;
    }
  }
  return badLED;
}

void loop() {
  while(true){
    Blynk.run();
    if (!isERROR){
      display.clear();  //清屏
      LED.write8(LEDCode[demoMode] & OFFLED);
      isERROR = scan()!=-1?true:false;
      if (isERROR) continue;
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
    else if(isERROR_1){
      ERROR();
      LED.write8(ERRORCode[m]);
      delay(50);
    }
    else { 
      ERROR();
      delay(50);
    }
  }
}
