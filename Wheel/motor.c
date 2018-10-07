#include<reg52.h> 
#include<intrins.h>  
#define uchar unsigned char 
#define uint unsigned int 
sbit Left_P=P1^0; 
sbit Left_D=P1^1;
sbit Right_P=P1^2;  
sbit Right_D=P1^3;
uchar pwm,speed=10,SerialValue=0;   //串口接收到的值; 
void delaynus(uchar n)
{
	uchar i;
 	for(i=0;i<n;i++);
}
void Init()
{
   	TMOD=0x21; 	//设定定时器为方式2 可自动再装入的定时器  
	SM0=0;  	//设定串口工作方式1 10位异步收发器  8位数据  
	SM1=1;  	//设定串口工作方式1 10位异步收发器  8位数据    
	REN=1;  	//允许串行接收位。REN=1，启动接收数据；REN=0，禁止接收。  
	IP=0x10;  
	TH0=(65536-100)/256;  
	TL0=(65536-100)%256;
	TH1=0xfd;	//装入初值  
	TL1=0xfd; 	//装入初值
	TR0=1;  	//启用T0定时器/计数器  
	ET0=1;  	//T0定时器中断开关；(开)   
	TR1=1;  	//启用T1定时器/计数器  
	ET1=1;  	//T1定时器中断开关；(开)  
	ES=1;  		//串行中断开关；(开)    
}
void main() 
{  
	P1=0x00;
	Init();
	EA=1;
	while(1)  
	{ 
		if(pwm<speed)
		{
			switch(SerialValue)
			{
			case 0:{P1=0x00;break;} //停止0000
			case 1:{P1=0x05;break;} //前进0101
			case 2:{P1=0x0a;break;} //后退1010
			case 3:{P1=0x09;break;} //左转1001
			case 4:{P1=0x06;break;} //右转0110
			default:break;
			}
			delaynus(2);
			speed--;
			if(speed<10)speed=10;
		}
		else P1=0x00;
				
	} 
}

void serial() interrupt 4 
{  
	if(RI)
	{
	SerialValue=SBUF;
	speed=30;    
	RI=0;
	}  
}   
void timer0() interrupt 1 
{   
	TH0=(65536-100)/256;  
	TL0=(65536-100)%256;  
	pwm++; 
 	if(pwm>100)pwm=0;     
}