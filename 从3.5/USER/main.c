#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "exti.h"
#include "mpu6050.h"  
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 
#include "key.h"
#include "adc.h"
#include "wkup.h"
#include "timer.h"
#include "rtc.h"
#include "stmflash.h"
#include <stdio.h>
typedef union
{	
	signed short intData;
	unsigned char byteData[2];
}INT_BYTE;


static int Rdflag=0;
u8 Count = 0;
u8 shutdown = 0;
u8 SleepFlag;
u8 alarm_set = 0;
u8 flashdata[11]={0x00};
void Send_datatoU2(int length ,u8 *s)
{
			int i=0;
      for(i=0;i<length;i++)
      {
				while((USART2->SR&0x0080)==0)
        {} ; //当数据还没有被转移到移位寄存器则等待
					USART2->DR=s[i];//发送一个字节的数据
      }
}
void Send_datatoU1(int length ,u8 *s)
{
			int i=0;
      for(i=0;i<length;i++)
      {
				while((USART1->SR&0x0080)==0)
        {} ; //当数据还没有被转移到移位寄存器则等待
					USART1->DR=s[i];//发送一个字节的数据
      }
}
int Getflag(void)
{
	if(Rdflag==1)
	{
		return 1;
	}	
	else
	{
		return 0;
	}	
	
}


float pitch,roll,yaw; 
float ADCtemp;
u16 ADdata = 0;
int main(void)
{
	INT_BYTE testpitch,testroll,testyaw;
	//u8 Reset[3]={0x02,0xC0,0x06};						//恢复出厂设置
	u8 Getadr[3]={0x02,0xC0,0x00};
	u8 temp[3];
	u8 Res;
	u8 ID=0x01;
	u8 jiaodu[7]={0x00,0x00,0x00,0x00,0x00,0x00,0x88};
	u8 head[6]={0x0c,0xc2,0x00,0x01,0x00,0x00};
	u8 test[7]={0x06,0xC2,0x00,0x10,0x01,0x02,0x03};
	u16 adcx;
	//float ADCtemp;
	int a=0;
	int j=0;
	int b=0;
    
	  
	  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	  USART1_Init(115200);    //串口1初始化
		USART2_Init(115200);    //串口2初始化
  	delay_init();	       //延时初始化 
		LED_Init();		  			//初始化与LED连接的硬件接口
	  KEY_Init();
	  Adc_Init();
  	EXTIX_Init();
	  BTPwrSet();
		MPU_Init();					//初始化MPU6050
    mpu_dmp_init();
	  RTC_Init();
	  LED1 = 0;	//LED亮	
	  LED2 = 1;
		while(mpu_dmp_init())
		{	
			delay_ms(20);			
		}
		LED1 = 1;delay_ms(200);
		LED1 = 0;delay_ms(80);
		LED1 = 1;delay_ms(200); 
		LED1 = 0;delay_ms(80); 
		LED1 = 1;delay_ms(200); 
		USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启串口2接受中断
		PWR_EN = 1;
		StopFlag = 0;
		SleepFlag = 0;
		delay_ms(500);
		TIM3_Int_Init(29999,7199);//10Khz的计数频率，计数到30000为3s 	
		LED1 = 0;	//LED灭
		STMFLASH_Read(FLASH_SAVE_ADDR,(u16*)flashdata,11);
		flashdata;
		
		//BTRestore();
		
		while(1)		//主循环
		{
			      if(Count > 20)
						{
							Count = 0;
							SleepFlag =1;
							RTC_Set(2021,1,1,0,0,0);
							alarm_set = 0;
						}
						if(SleepFlag == 1){
							LED1 = 1;
							LED2 = 1;
							TIM_Cmd(TIM3, DISABLE);  //关闭TIMx
							if(alarm_set == 0){
								RTC_Alarm_Set(2021,1,1,0,20,0);
								alarm_set = 1;
							}
							__WFI();
						}
//			      if(StopFlag == 1){
//						LED1 = 1;
//						LED2 = 1;
//						MPU_Write_Byte(MPU_PWR_MGMT1_REG,0X41);
//						Sys_Enter_Stop();
//					}	
					delay_ms(100);
          //adcx=Get_Adc_Average(ADC_Channel_0,10);
					adcx = GetADC();
			    ADCtemp=(float)adcx*(3.3/4096)*9/4;
					
					if(CHRG == 0){
						Count = 0;
						if(ADCtemp > CHRG_stage1){
							if(ADCtemp > FULLCHRG){
                   LED1 = 0;         //充满电
								   LED2 = 1;
						    }
						  else{
							  LED1 = 0;            //充电阶段1
								LED2 = 0;
						  }
			      }
						else{
							 LED2 = 0;            //充电阶段0
						}
			   }
					else{
						  if(ADCtemp < LOWPWR){
								if(ADCtemp < Very_LOWPWR){
									PWR_EN = 0;
								}
								else{
								LED1 = 1;
							 delay_ms(80); LED2 = 0;	 	//LED亮
		           delay_ms(80); LED2 = 1;		 	//LED灭
		           delay_ms(80); LED2 = 0;	 	//LED亮
		           LED2 = 1;										//LED灭
								}
							}
							else{
								LED1 = 0;
								LED2 = 1;
							}
					}

		     if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)                        
		     {
					  delay_ms(100);
		     }
				 
				 if(USART_GetFlagStatus(USART2, USART_FLAG_RXNE) != RESET)//读数据寄存器非空
				 {  
					Res=USART_ReceiveData(USART2);	
					if(Res==0x03)
					{
						a=1;	
					}
					if((a==1)&&(Res==0x43))
					{
						a=2;
					}	
					if((a==2)&&(Res==0x15))
					{
						a=3;
					}	
					if((a==3)&&(Res==0x00))
					{
						a=0;
						break;
					}	
				 }
		}	
				delay_ms(100);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启串口2接受中断
	while(1){}
}
