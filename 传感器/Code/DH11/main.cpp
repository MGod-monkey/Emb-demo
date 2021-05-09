#include <Adafruit_Sensor.h>        //传感器详情库
#include <DHT.h>        //温湿度传感器库
#include <DHT_U.h>

#define DHTPIN 33     // 温湿度传感器的数据引脚（自行更改）

// 使用的温湿度传感器类型
#define DHTTYPE    DHT11     // DHT 11
// #define DHTTYPE    DHT22     // DHT 22 (AM2302)
//#define DHTTYPE    DHT21     // DHT 21 (AM2301)

//定义传感器dht
DHT_Unified dht(DHTPIN, DHTTYPE);

uint32_t delayMS;       //读数据延时

void setup() {
  Serial.begin(115200);
  // 初识化设备
  dht.begin();
  Serial.println(F("DHTxx Unified Sensor Example"));
  // 打印温湿度传感器详情
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  Serial.println(F("------------------------------------"));
  Serial.println(F("温度传感器"));
  Serial.print  (F("传感器类型: ")); Serial.println(sensor.name);
  Serial.print  (F("驱动版本: ")); Serial.println(sensor.version);
  Serial.print  (F("ID号: ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("测量最大温度:  ")); Serial.print(sensor.max_value); Serial.println(F("°C"));
  Serial.print  (F("测量最小温度:  ")); Serial.print(sensor.min_value); Serial.println(F("°C"));
  Serial.print  (F("测量精度: ")); Serial.print(sensor.resolution); Serial.println(F("°C"));
  Serial.println(F("------------------------------------"));
  // Print humidity sensor details.
  dht.humidity().getSensor(&sensor);
  Serial.println(F("湿度传感器"));
  Serial.print  (F("传感器类型: ")); Serial.println(sensor.name);
  Serial.print  (F("驱动版本: ")); Serial.println(sensor.version);
  Serial.print  (F("ID号: ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("测量最大湿度:  ")); Serial.print(sensor.max_value); Serial.println(F("%"));
  Serial.print  (F("测量最小湿度:  ")); Serial.print(sensor.min_value); Serial.println(F("%"));
  Serial.print  (F("测量精度: ")); Serial.print(sensor.resolution); Serial.println(F("%"));
  Serial.println(F("------------------------------------"));
  // 根据传感器的详细信息设置两次传感器读数之间的延迟
  delayMS = sensor.min_delay / 1000;
}

void loop() {
  delay(delayMS);
  // 获取传感器数据事件
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println(F("读取温度失败!"));
  }
  else {
    Serial.print(F("Temp: "));
    Serial.print(event.temperature);
    Serial.println(F("°C"));
  }
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println(F("读取湿度失败!"));
  }
  else {
    Serial.print(F("Hum: "));
    Serial.print(event.relative_humidity);
    Serial.println(F("%"));
  }
}
