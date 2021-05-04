#define SPin 16  //定义按键引脚
#define LED 2  //定义LED引脚
#define ONRes 100 //LED亮的最高阻值

int resValue = 0; //光敏电阻阻值

/*
传感器：光线/光敏电阻
实验现象：读取光敏电阻的阻值，打印到串口台，当阻值低于100Ω时，LED发亮
*/

void setup() {
  //定义串口波特率
  Serial.begin(115200);
  //定义引脚模式
  pinMode(LED, OUTPUT);
}

void loop() {
  resValue = analogRead(SPin);
  Serial.printf("now RES: %d\n", resValue);
  if(resValue <= 100)   digitalWrite(LED, LOW);
  else digitalWrite(LED, HIGH);
  delay(50);
}