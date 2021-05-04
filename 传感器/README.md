# 37中传感器套件简单使用案例

> 最近若没啥好玩的项目，应该会持续更新传感器的套件简单使用案例



- 开发环境

  VScode+platformIO / Arduino （搭建教程:[博客](https://blog.csdn.net/qq_45516773/article/details/115406420)）

- 单片机

  ESP8266

- 传感器

  37种常用的传感器（淘宝链接:[link](https://item.taobao.com/item.htm?spm=a230r.1.14.22.6f0ecc97YmzU1o&id=607237908874&ns=1&abbucket=9#detail)）



传感器案例分类:

| 传感器 | 图片 | 所用到的第三方库 | 代码 |引脚说明|
| :----: | :--: | :--------------: | :--: | :--- |
|  按键  | ![](https://i.loli.net/2021/05/04/ozVDcb28F5luxvH.png) |        无        |      | Pin1->D0(GPIO16)<br>Pin3->GND |
| 光敏电阻 | ![image-20210504135026890](https://i.loli.net/2021/05/04/PKhvbTyY2eMOWEo.png) | 无 | | Pin1->D0(GPIO16)<br>Pin2->3.3V<br>Pin3->GND |
| 红外接收 | ![image-20210504135307851](https://i.loli.net/2021/05/04/pRsgcMuhX7IHdjm.png) |  | |  |
| 火焰传感器 | ![image-20210504135324968](https://i.loli.net/2021/05/04/5VkYus74w8xWnji.png) |  | |  |
| 温度传感器 | ![image-20210504135339469](https://i.loli.net/2021/05/04/mE5nkT14KbvclNx.png) |  | |  |
| 麦克风 | ![image-20210504135415436](https://i.loli.net/2021/05/04/QGly5IAcvJmktxh.png) |  | |  |
| 磁环 | ![image-20210504135443480](https://i.loli.net/2021/05/04/pwBqiRh25mn74uE.png) |  | |  |
| 红外发射 | ![image-20210504135509083](https://i.loli.net/2021/05/04/fVGrmq6pT1h9Rse.png) |  | |  |

使用说明：

