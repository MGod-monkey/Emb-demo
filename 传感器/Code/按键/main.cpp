#define K1 16  //定义按键引脚
#define LED 2  //定义LED引脚
#define DelayTime 500 //定义闪烁间隔时间
int state = 0; //定义状态

/*
传感器：按键
实验现象：按一次按键，ESP8266板子上的LED进行：开->关->闪烁->关 循环状态切换
*/
void onChange() {
    state = (state + 1) % 4;
}

void setup() {
    //定义引脚模式
    pinMode(LED, OUTPUT);
    pinMode(K1, INPUT_PULLUP);
    //定义按键中断
    attachInterrupt(K1, onChange, FALLING);
}

void loop() {
    switch (state)
    {
    case 0:
        digitalWrite(LED, LOW);
        break;
    case 2:
        digitalWrite(LED, LOW);
        delay(DelayTime);
        digitalWrite(LED, HIGH);
        delay(DelayTime);
        break;
    default:
        digitalWrite(LED, HIGH);
        break;
    }
    delay(10);
}