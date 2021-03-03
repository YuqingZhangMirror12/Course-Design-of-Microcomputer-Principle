#include <reg51.h>
#define uint unsigned int
#define uchar unsigned char			   //头文件定义

sbit GREEN_H = P0^0;			       // 定义水平方向horizontal对应的引脚
sbit YELLOW_H = P0^1;
sbit RED_H = P0^2;

sbit YELLOW_V = P0^3;		           // 定义竖直方向vertical对应的引脚
sbit GREEN_V = P0^4;
sbit RED_V = P0^5;

sbit KEY=P3^2;	                      
sbit nbkey=P1^0;					  //定义手动模式下的单行道控制变量
sbit dxkey=P1^1;


uchar pause=0;

uchar Operation_Type = 1;			 //定义自动模式下的模式切换变量
uchar Time_Count=0;

 void Delays(unsigned int s)		//定义延时子程序
{
	unsigned int i,j,k;
	for(i=0;i<ms;i++)  
		for(j=0;j<15000;j++)
		for (k=0;k<9;k++); 
}

 void main()
{		TH0=h;							  //计时器及外部中断初始化定义
	TL0=l ;

    IP=	 0X04;
	IE = 0x87;
 	TMOD = 0x01;
 	TCON = 0x11;	
        nbkey=1;
        dxkey=1;
TH0=h;
TL0=l;


	while(1)							   //手动切换模块，用查询进行方式切换
{ if(nbkey==0)                           		 //按键nbkey按下，竖直方向通车，水平方向禁行
        { TR0=0; 
        YELLOW_H=1;
        YELLOW_V=1;
        GREEN_H=1;
		GREEN_V=0;
        RED_H=0;
        RED_V=1;
        }
       
 if(dxkey==0)                          			  //按键dxkey按下，水平方向通车，竖直方向禁行
                  
        { TR0=0;        
        YELLOW_H=1;
        YELLOW_V=1;
        GREEN_H=0;
		GREEN_V=1;
        RED_H=1;
        RED_V=0;
        }
       

}
        }

void External_Interrupt_0() interrupt 0    //自动-手动模式转换，通过中断方式进行
     {TR0=0;
     
	  pause=1;
	 }

void External_Interrupt_1() interrupt 2     
{  TR0=1;
	pause=0;  
      }

void T0_INT() interrupt 1                 //自动切换模式
{
 	 if(pause==1) return;
	switch(Operation_Type)
	{
	 	case 1:								 //水平方向通行，竖直方向禁行，持续5秒
			RED_H=1;YELLOW_H=1;GREEN_H=0;
			RED_V=0;YELLOW_V=1;GREEN_V=1;
           	Delays(5);
			Operation_Type=2;
				return;		

		case 2:								  //水平方向黄灯等待，竖直方向禁行，持续2秒
			RED_H=1;YELLOW_H=0;GREEN_H=1;
			RED_V=0;YELLOW_V=1;GREEN_V=1;          
			Delays(2);
			Operation_Type=3;
		 return;


		case 3:								   //水平方向禁行，竖直方向通行，持续5秒
			RED_H=0;YELLOW_H=1;GREEN_H=1;
			RED_V=1;YELLOW_V=1;GREEN_V=0;            
			Delays(5);
			Operation_Type=4;
				 return;

		case 4:								  //水平方向禁行，竖直方向黄灯等待，持续2秒
			RED_H=0;YELLOW_H=1;GREEN_H=1;
			RED_V=1;YELLOW_V=0;GREEN_V=1;            
			Delays(2);
			Operation_Type=1;
				 return;
		
	}

}
