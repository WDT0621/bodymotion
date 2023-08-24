#include "stm32f10x.h"
#include "key.h"
#include "sys.h" 
#include "delay.h"
#include "led.h"

//按键初始化函数
void KEY_Init(void) //IO初始化
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//使能PORTB时钟

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_15;//BT_RES->PA.15
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.15

	//初始化 KEY_ON-->GPIOB.10	  下拉输入
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PB10设置成输入，默认下拉	  
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOB.10

	BKP_TamperPinCmd(DISABLE);//关闭入侵检测功能，也就是 PC13，也可以当普通IO 使用
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;            //INT
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	       //上拉输入
  GPIO_Init(GPIOC, &GPIO_InitStructure);               //初始化PC.13
}

u8 Check_WKUP(void) 
{
	u8 t=0;	//记录按下的时间
	LED1 = 1; //关闭LED
	while(1)
	{
		if(KEY_ON == 1)
		{
			t++;			//已经按下了 
			delay_ms(30);
			if(t>=100)		//按下超过3秒钟
			{
				return 1; 	//按下3s以上了
			}
		}else 
		{ 
			//LED2=0;
			//wakeup
			return 0; //按下不足3秒
		}
	}
}

u8 Check_Res(void) 
{
	u8 t=0;	//记录按下的时间
	LED2 = 1;
	LED1 = 1; //关闭LED
	while(1)
	{
		if(BT_RES == 0)
		{
			t++;			//已经按下了 
			delay_ms(30);
			if(t>=100)		//按下超过3秒钟
			{
				return 1; 	//按下3s以上了
			}
		}else 
		{ 
			return 0; //按下不足3秒
		}
	}
}