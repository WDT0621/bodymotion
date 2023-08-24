#include "exti.h"
#include "led.h"
#include "key.h"
#include "delay.h"
#include "usart.h"
#include "wkup.h"
#include "mpu6050.h"  
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 
u8 StopFlag = 0;
extern u8 SleepFlag;
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//�ⲿ�ж� ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/3
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////   
//�ⲿ�ж�0�������
void EXTIX_Init(void)
{
 
 	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;

    KEY_Init();	 //	�����˿ڳ�ʼ��
    //MPU_Motion_Init();
  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//ʹ�ܸ��ù���ʱ��


	 //GPIOA.15	  �ж����Լ��жϳ�ʼ������  �½��ش���	//BT_RES
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource15);
	  EXTI_InitStructure.EXTI_Line=EXTI_Line15;	//BT_RES
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���


	 //GPIOB.10	  �ж����Լ��жϳ�ʼ������ �����ش��� PB10  KEY_ON
	  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource10); 
  	EXTI_InitStructure.EXTI_Line=EXTI_Line10;
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  	EXTI_Init(&EXTI_InitStructure);		//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
		
		//GPIOB.11	  �ж����Լ��жϳ�ʼ������ �����ش��� PB11  CHRG_WKUP
	  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource11); 
  	EXTI_InitStructure.EXTI_Line=EXTI_Line11;
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  	EXTI_Init(&EXTI_InitStructure);		//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
		
		//GPIOC.13	  �ж����Լ��жϳ�ʼ������ �����ش��� PC13  MPU_INT
//	  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource13); 
//  	EXTI_InitStructure.EXTI_Line=EXTI_Line13;
//  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
//  	EXTI_Init(&EXTI_InitStructure);		//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

	  NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;			//ʹ�ܰ���KEY_ON���ڵ��ⲿ�ж�ͨ��
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;					//�����ȼ�3 
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure);  	  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

}

void EXTI15_10_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line15)!=RESET ){
	delay_ms(10);//����
	if(Check_Res() == 1)
	{
		//BTRestore();
	}		 
	EXTI_ClearITPendingBit(EXTI_Line15);  //���LINE15�ϵ��жϱ�־λ  
}
	if(EXTI_GetITStatus(EXTI_Line10)!=RESET ){
		delay_ms(10);
		if(Check_WKUP() == 0){		  //�ػ���
			
//			Count = 0;
//			if(SleepFlag == 1){
//				SleepFlag = 0;
//				TIM_Cmd(TIM3, ENABLE);
//				  LED1 = 0;delay_ms(80);
//		      LED1 = 1;delay_ms(200); 
//		      LED1 = 0;delay_ms(300); 
//		      LED1 = 1;delay_ms(200); 
//				  LED1 = 0;delay_ms(1200);
//		      LED1 = 1;
//			}
//			else{
//				StopFlag = !StopFlag;
//			  if(StopFlag == 1){
//				  LED1 = 0;delay_ms(500);
//		      LED1 = 1;delay_ms(200); 
//		      LED1 = 0;delay_ms(200); 
//		      LED1 = 1;delay_ms(200); 
//				  LED1 = 0;delay_ms(80);
//		      LED1 = 1;
//			   }
//			  else{
//					Wkup_Init();
//			  }
//			}
	  }
		else{
				LED1 = 0;delay_ms(9000);
        PWR_EN = 0;
				LED1 = 1;delay_ms(5000);		//�ػ�			
		} 
	  EXTI_ClearITPendingBit(EXTI_Line10);  //���LINE10�ϵ��жϱ�־λ 
	}
		if(EXTI_GetITStatus(EXTI_Line11)!=RESET){
			SleepFlag = 0;
			StopFlag = 0;
			EXTI_ClearITPendingBit(EXTI_Line11);
		}
 }
	
