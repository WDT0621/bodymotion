#include "wkup.h"
#include "delay.h"
#include "exti.h"
#include "led.h"
#include "adc.h"
#include "timer.h"
#include "usart.h"
#include "mpu6050.h"  
#include "inv_mpu.h"
#include "rtc.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//待机唤醒 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/7
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
	 
void Sys_Stop(void)
{  
	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);	//使能PWR外设时钟
	 PWR_EnterSTOPMode(PWR_Regulator_LowPower,PWR_STOPEntry_WFI);	  //进入停机（STOP）模式 		 
}


void DisablePeriph(void)
{
   GPIO_InitTypeDef GPIO_InitStructure;
	
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	 //GPIO_Init(GPIOA, &GPIO_InitStructure);	
	 GPIO_Init(GPIOB, &GPIO_InitStructure);
	 GPIO_Init(GPIOC, &GPIO_InitStructure);                //所有端口设定为模拟输入
	
	
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3;	 //串口
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
   GPIO_Init(GPIOA, &GPIO_InitStructure);					       //根据设定参数初始化

	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;	           //PWR_EN-->PA.6 端口配置
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
   GPIO_Init(GPIOA, &GPIO_InitStructure);					       //根据设定参数初始化GPIOA.6
	 GPIO_SetBits(GPIOA,GPIO_Pin_6);		                   //保持供电
	
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	           //KEY_ON-->PB.10 端口配置
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;         //下拉输入
   GPIO_Init(GPIOB, &GPIO_InitStructure);					       //根据设定参数初始化GPIOB.10

	 /*PD*/    //如果使用了外部晶振，也要进行配置
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1; 
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	 GPIO_ResetBits(GPIOD,GPIO_Pin_0|GPIO_Pin_1);
	 GPIO_Init(GPIOD, &GPIO_InitStructure);
}


void GPIO_Init_Wkup(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE);       //打开GPIO时钟
	/*串口初始化*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3; //TXD2 RXD2
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	     //复用推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);               //初始化GPIOA.2 PA.3
	/*蓝牙复位按键*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;            //BT_RES
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	        //上拉输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);                //初始化PA15
	/*BOOT1*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;            //BOOT1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	       //下拉输入
  GPIO_Init(GPIOB, &GPIO_InitStructure);               //初始化PB2
	/*LED*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7; //LED1 LED2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;	   //开漏输出
  GPIO_Init(GPIOB, &GPIO_InitStructure);               //PB6 PB7
	GPIO_SetBits(GPIOB,GPIO_Pin_6);						           //PC.6 输出低 LED1亮
  GPIO_SetBits(GPIOB,GPIO_Pin_7);						           //PC.7 输出高 LED2灭
	/*IIC*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9; //SCL、SDA
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	   //通用推挽
  GPIO_Init(GPIOB, &GPIO_InitStructure);               //PB8 PB9
	/*电源使能*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;            //PWR_EN-->PA.6
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	    //通用推挽
  GPIO_Init(GPIOA, &GPIO_InitStructure);                //初始化PA.6
	GPIO_SetBits(GPIOA,GPIO_Pin_6);						          //PA.6 输出高 持续供电
	/*开关机按键*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;            //KEY_ON-->PB.10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	       //下拉输入
  GPIO_Init(GPIOB, &GPIO_InitStructure);               //初始化PB10
	/*6050中断*/
	BKP_TamperPinCmd(DISABLE);                           //关闭入侵检测功能，也就是 PC13，也可以当普通IO 使用
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;            //INT
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	       //下拉输入
  GPIO_Init(GPIOC, &GPIO_InitStructure);               //初始化PC.13
	/*蓝牙电源*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;            //BT_PER->PB.3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	   //通用推挽输出
  GPIO_Init(GPIOB, &GPIO_InitStructure); 
	GPIO_ResetBits(GPIOB,GPIO_Pin_3);	                   //打开蓝牙电源
	/*充电标志*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;            //CHRG_WKUP->PB.11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	       //下拉输入
  GPIO_Init(GPIOB, &GPIO_InitStructure);               //初始化PB.11
}

void Wkup_Init(void)
{
		SYSCLKConfig_STOP();  //唤醒并启用HSE
		GPIO_Init_Wkup();
	  USART2_Init(115200);    //串口2初始化为115200
		USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启串口2接受中断
	  MPU_Write_Byte(MPU_PWR_MGMT1_REG,0X01);  //使能MPU6050
	  RTC_Init();
		LED1 = 0;delay_ms(80);
		LED1 = 1;delay_ms(200); 
		LED1 = 0;delay_ms(300); 
		LED1 = 1;delay_ms(200); 
		LED1 = 0;delay_ms(800);
		LED1 = 1;
}

void DisableBT()
{
	 GPIO_InitTypeDef GPIO_InitStructure;
	
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;	           //BT_PWR-->PB.3 端口配置
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;      //通用推挽输出
   GPIO_Init(GPIOB, &GPIO_InitStructure);					       //根据设定参数初始化GPIOB.3
	 GPIO_SetBits(GPIOB,GPIO_Pin_3);	                     //输出高，蓝牙关闭
}
//系统进入停机模式
void Sys_Enter_Stop(void)
{			 
	 RCC_APB2PeriphResetCmd(0X01FC,DISABLE);	//复位所有IO口
	 DisablePeriph();
	 DisableBT();
	 Sys_Stop();
}

/**
  * @brief  停机唤醒后配置系统时钟: 使能 HSE, PLL
  *         并且选择PLL作为系统时钟.
  * @param  None
  * @retval None
  */
void SYSCLKConfig_STOP(void)
{
  /* After wake-up from STOP reconfigure the system clock */
  /* 使能 HSE */
  RCC_HSEConfig(RCC_HSE_ON);
  
  /* 等待 HSE 准备就绪 */
  while (RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET)
  {
  }
  
  /* 使能 PLL */ 
  RCC_PLLCmd(ENABLE);
  
  /* 等待 PLL 准备就绪 */
  while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
  {
  }
  
  /* 选择PLL作为系统时钟源 */
  RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
  
  /* 等待PLL被选择为系统时钟源 */
  while (RCC_GetSYSCLKSource() != 0x08)
  {
  }
}
