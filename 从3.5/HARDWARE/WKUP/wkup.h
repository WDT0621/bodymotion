#ifndef __WKUP_H
#define __WKUP_H	 
#include "sys.h"
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
					    
void Sys_Stop(void);	 
void Sys_Enter_Stop(void);	//系统进入停机模式
void SYSCLKConfig_STOP(void);   //停机唤醒后配置系统时钟: 使能 HSE, PLL
void Wkup_Init(void);
void GPIO_Init_Wkup(void);
#endif


