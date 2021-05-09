# 37中传感器套件简单使用案例

> 最近若没啥好玩的项目，应该会持续更新传感器的套件简单使用案例



- 开发环境

  VScode+platformIO / Arduino （搭建教程:[博客](https://blog.csdn.net/qq_45516773/article/details/115406420)）

- 单片机

  ESP32

- 传感器

  37种常用的传感器（淘宝链接:[link](https://item.taobao.com/item.htm?spm=a230r.1.14.22.6f0ecc97YmzU1o&id=607237908874&ns=1&abbucket=9#detail)）



传感器案例分类:

| 传感器 | 图片 | 所用到的第三方库 | 代码 |引脚说明|
| :----: | :--: | :--------------: | :--: | :--- |
|  按键  | ![](https://i.loli.net/2021/05/04/ozVDcb28F5luxvH.png) |        无        | [code](https://github.com/MGod-monkey/Emb-demo/blob/master/%E4%BC%A0%E6%84%9F%E5%99%A8/Code/key.ino) | Pin1->D0(GPIO16)<br>Pin3->GND |
| 光敏电阻 | ![image-20210504135026890](https://i.loli.net/2021/05/04/PKhvbTyY2eMOWEo.png) | 无 | [code](https://github.com/MGod-monkey/Emb-demo/blob/master/%E4%BC%A0%E6%84%9F%E5%99%A8/Code/photoresistor.ino) | Pin1->D0(GPIO16)<br>Pin2->3.3V<br>Pin3->GND |
| 红外接收 | ![image-20210504135307851](https://i.loli.net/2021/05/04/pRsgcMuhX7IHdjm.png) |[IRremoteESP8266](https://github.com/crankyoldgit/IRremoteESP8266) | | Pin1->D0(GPIO16)<br>Pin2->3.3V<br>Pin3->GND |
| 火焰传感器 | ![image-20210504135324968](https://i.loli.net/2021/05/04/5VkYus74w8xWnji.png) |  | |  |
| 温度传感器 | ![image-20210504135339469](https://i.loli.net/2021/05/04/mE5nkT14KbvclNx.png) |  | |  |
| 麦克风 | ![image-20210504135415436](https://i.loli.net/2021/05/04/QGly5IAcvJmktxh.png) |  | |  |
| 磁环 | ![image-20210504135443480](https://i.loli.net/2021/05/04/pwBqiRh25mn74uE.png) |  | |  |
| 红外发射 | ![image-20210504135509083](https://i.loli.net/2021/05/04/fVGrmq6pT1h9Rse.png) |[IRremoteESP8266](https://github.com/crankyoldgit/IRremoteESP8266) |  | Pin1->D0(GPIO16)<br/>Pin2->3.3V<br/>Pin3->GND |

>``注:元器件朝上，引脚朝下，从左到右依次为Pin1，Pin2···``



使用说明

VScode+platformIO:

1. 新建项目工程

2. 复制配置文件`platformio.ini`到工程目录下`/platformio.ini`中

3. 复制`main.cpp`到工程目录下`/src/main.cpp`中

4. 连线->编译->上传

> **注：单片机以ESP32为主，若使用ESP8266，则需要修改配置文件和main中对应的引脚**

Arduino:

1. 从`工具->加载库->管理库`中添加第三方库
   
2. 通过`文件->新建文件`创建文件，复制`main.cpp`到文件中
   
3. 连线->编译->上传