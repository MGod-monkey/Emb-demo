#include "reg52.h"

typedef unsigned int u16;	  
typedef unsigned char u8;


//接口定义
#define SMG_DATA P0	//数码管数据接口
#define DAC_DATA P1	//dac数据接口
sbit LSA=P2^6;
sbit LSB=P2^5;
sbit LSC=P2^4;
sbit K1=P2^0;	//加键
sbit K2=P2^1;	//减键
sbit K3=P2^2;	//归零键
sbit K4=P2^3;	//息屏键
sbit beep=P2^7;	//蜂鸣器
u8 code smg[11]={0xd7,0x14,0xcd,0x5d,0x1e,0x5b,0xdb,0x15,0xdf,0x5f};	//阴极数码管段码
//u8 code smg[17]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,
					//0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};//显示0~F的值

//全局变量定义
u8 w;	//位选值
u8 is_display=1;	//是否开启屏显，默认开启
u8 now_v=0,STEP=5;	//当前的电压值=now_v/10+20,步进位5
u8 delay_time=150;	//按键识别为长按间隔时间:1.5s
u8 count1,count2,start=0,count_v=0;		//定时器变量


//延时函数
void delay_ms(u16 xms)
{
	u16 x,y;
	for(x=xms;x>0;x--)
		for(y=110;y>0;y--);
}
//阴极数码管显示事件
void Display(){
	if(is_display==1){
	for(w=0;w<3;w++)
	{
		switch(w)
		{
			case(0):LSC=1;LSA=0;SMG_DATA=smg[(now_v+20)/100];delay_ms(5);SMG_DATA=0x00;break;			//十位数
			case(1):LSA=1;LSB=0;SMG_DATA=smg[(now_v+20)%100/10]^0x80;delay_ms(5);SMG_DATA=0x00;break;	//个位数，第三位显示小数点
			case(2):LSB=1;LSC=0;SMG_DATA=smg[(now_v+20)%10];delay_ms(5);SMG_DATA=0x00;break;			//十分位
		}
	}
	}
	else{		//关闭显示功能
	LSA=LSB=LSC=0;
	SMG_DATA=0x00;
	}
}
//键盘扫描事件
void KeyScan(){
	if(K1!=1||K2!=1||K3!=1|K4!=1){
		delay_ms(100);//消抖
		if(K1==0){	//加键被按下
			count1=count2=0;
			if(now_v<160){
				now_v+=STEP;
				DAC_DATA=now_v;
				while(!K1){	//等待按键松开
					Display();
					if(start&&count_v>0&&now_v<160){
					now_v+=count_v;
					count_v=0;
					}
					DAC_DATA=now_v;
				}
		}
			else{
			DAC_DATA=now_v=160;
			beep=1;
			while(!K1){	//等待按键松开
			Display();}
			}
			start=0;	//将长按功能关闭
			beep=0;
	}
		//电压减
			if(K2==0){
				count1=count2=0;
				if(now_v>0){ 	//now_v值不为0
				now_v-=STEP;
				DAC_DATA=now_v;
				while(!K2){	//等待按键松开
					Display();
				if(start&&count_v>0&&now_v>0){
					now_v-=count_v;
					count_v=0;
					}
					DAC_DATA=now_v;
				}
			}
				else{
					DAC_DATA=now_v=0;
					beep=1;		//蜂鸣器开始警告
					while(!K2){	//等待按键松开
					Display();}
				}
				start=0;	//将长按功能关闭
				beep=0;
			}
		//清零
		if(K3==0){
		while(now_v>=10){
			now_v-=10;
			DAC_DATA=now_v;
			delay_ms(20);
			Display();
		}
		DAC_DATA=now_v=0;
		Display();
		while(!K3){		//等待按键松开
			Display();
		}
	}
		//息屏或启屏
		if(K4==0){
		is_display=~is_display;
		while(!K4);}
	}
}
//初始化定时器0
void InitTimer0(void)
{
    TMOD=0x01;//1.模式设置，00000001，采用的是定时器0，工作与式1（M1=0，M0=1）。
    TH0=(65536-10000)/256;  //2.定时器设置，每隔10毫秒发起一次中断。
    TL0=(65536-10000)%256;
    ET0=1;                  //3.开定时器0中断
    EA = 1;                 //4.开总中断
    TR0=1;                  //5.打开定时器
}
void main()
{	
	InitTimer0();
	beep=0;
	while(1)
	{	
		Display();
		KeyScan();
}
	}

void Timer0Interrupt(void) interrupt 1
{
    TH0=(65536-10000)/256;  //2.定时器设置，每隔10毫秒发起一次中断。
    TL0=(65536-10000)%256;
	count1++;	//10ms加一数
	if(count1>=delay_time){
		count1=0;	//清零，防止内存溢出
		start=1;	//长按1.5s以上，按键为长按状态，开始隔0.5s累加1
	}
	if(start) count2++;
	if(count2>=50){count2=0;count_v=STEP;}
}