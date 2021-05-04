#include <IRremoteESP8266.h>
#include <IRsend.h>

#define IRLed 16  //定义红外发射引脚
#define LED 2
IRsend irsend(IRLed); //定义红外发射模块

void setup() {
  irsend.begin();
  pinMode(LED, OUTPUT);
 Serial.begin(115200, SERIAL_8N1, SERIAL_TX_ONLY);
 //Serial.begin(115200);
}

void loop() {
  Serial.println("Sony");
  irsend.sendSony(0xa90, 12, 2);  //发送Sony协议指令
  /*
   * 类似的通行协议有：
   * irsend.sendNEC();  NEC协议指令
   * irsend.sendSony(); Sony协议指令
   * irsend.sendRC5();  Philips RC5协议指令
   * irsend.sendRC6();  Philips RC6协议指令
   * irsend.sendSharp();  Sharp协议指令
   * irsned.sendPanasonic();  Panasonic协议指令
   * irsned.sendJVC();   JVC协议指令
   * irsend.sendRaw();  原始指令
   */
  for (int i=0; i<10; i++){
    digitalWrite(LED, !digitalRead(LED));
    delay(100);}
  delay(1000);
}