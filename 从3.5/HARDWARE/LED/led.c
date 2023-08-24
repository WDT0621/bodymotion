#include "led.h"
    
//LED IO初始化
void LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOA, ENABLE);	 //使能PA、PB端口时钟
	
	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOF, ENABLE); 
 GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;				     //LED1-->PB.6 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 		 //开漏输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);					       //根据设定参数初始化GPIOB.6
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;	           //LED2-->PB.7 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;     //开漏输出
 GPIO_Init(GPIOB, &GPIO_InitStructure);					       //根据设定参数初始化GPIOB.7
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;	           //PWR_EN-->PA.6 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
 GPIO_Init(GPIOA, &GPIO_InitStructure);					       //根据设定参数初始化GPIOA.6
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;				     //CHRG_WKUP-->PB.11 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 		     //下拉输入
 GPIO_Init(GPIOB, &GPIO_InitStructure);					       //根据设定参数初始化GPIOB.11
 
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;				     //BT_PWR-->PB.3 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		 //
 GPIO_Init(GPIOB, &GPIO_InitStructure);					       //根据设定参数初始化GPIOB.11
	
 GPIO_SetBits(GPIOB,GPIO_Pin_6);						           //PC.6 输出高 LED1灭
 GPIO_SetBits(GPIOB,GPIO_Pin_7);						           //PC.7 输出高 LED2灭
 GPIO_ResetBits(GPIOB,GPIO_Pin_3);						         //PB.3 输出低 BT使能
 GPIO_ResetBits(GPIOB,GPIO_Pin_10);						         //PC.10 输出低 PWR不使能

}
 
