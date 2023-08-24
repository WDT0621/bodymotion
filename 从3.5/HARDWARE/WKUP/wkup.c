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
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//�������� ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/7
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
	 
void Sys_Stop(void)
{  
	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);	//ʹ��PWR����ʱ��
	 PWR_EnterSTOPMode(PWR_Regulator_LowPower,PWR_STOPEntry_WFI);	  //����ͣ����STOP��ģʽ 		 
}


void DisablePeriph(void)
{
   GPIO_InitTypeDef GPIO_InitStructure;
	
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	 //GPIO_Init(GPIOA, &GPIO_InitStructure);	
	 GPIO_Init(GPIOB, &GPIO_InitStructure);
	 GPIO_Init(GPIOC, &GPIO_InitStructure);                //���ж˿��趨Ϊģ������
	
	
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3;	 //����
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
   GPIO_Init(GPIOA, &GPIO_InitStructure);					       //�����趨������ʼ��

	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;	           //PWR_EN-->PA.6 �˿�����
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
   GPIO_Init(GPIOA, &GPIO_InitStructure);					       //�����趨������ʼ��GPIOA.6
	 GPIO_SetBits(GPIOA,GPIO_Pin_6);		                   //���ֹ���
	
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	           //KEY_ON-->PB.10 �˿�����
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;         //��������
   GPIO_Init(GPIOB, &GPIO_InitStructure);					       //�����趨������ʼ��GPIOB.10

	 /*PD*/    //���ʹ�����ⲿ����ҲҪ��������
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1; 
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	 GPIO_ResetBits(GPIOD,GPIO_Pin_0|GPIO_Pin_1);
	 GPIO_Init(GPIOD, &GPIO_InitStructure);
}


void GPIO_Init_Wkup(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE);       //��GPIOʱ��
	/*���ڳ�ʼ��*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3; //TXD2 RXD2
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	     //�����������
  GPIO_Init(GPIOA, &GPIO_InitStructure);               //��ʼ��GPIOA.2 PA.3
	/*������λ����*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;            //BT_RES
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	        //��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);                //��ʼ��PA15
	/*BOOT1*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;            //BOOT1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	       //��������
  GPIO_Init(GPIOB, &GPIO_InitStructure);               //��ʼ��PB2
	/*LED*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7; //LED1 LED2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;	   //��©���
  GPIO_Init(GPIOB, &GPIO_InitStructure);               //PB6 PB7
	GPIO_SetBits(GPIOB,GPIO_Pin_6);						           //PC.6 ����� LED1��
  GPIO_SetBits(GPIOB,GPIO_Pin_7);						           //PC.7 ����� LED2��
	/*IIC*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9; //SCL��SDA
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	   //ͨ������
  GPIO_Init(GPIOB, &GPIO_InitStructure);               //PB8 PB9
	/*��Դʹ��*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;            //PWR_EN-->PA.6
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	    //ͨ������
  GPIO_Init(GPIOA, &GPIO_InitStructure);                //��ʼ��PA.6
	GPIO_SetBits(GPIOA,GPIO_Pin_6);						          //PA.6 ����� ��������
	/*���ػ�����*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;            //KEY_ON-->PB.10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	       //��������
  GPIO_Init(GPIOB, &GPIO_InitStructure);               //��ʼ��PB10
	/*6050�ж�*/
	BKP_TamperPinCmd(DISABLE);                           //�ر����ּ�⹦�ܣ�Ҳ���� PC13��Ҳ���Ե���ͨIO ʹ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;            //INT
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	       //��������
  GPIO_Init(GPIOC, &GPIO_InitStructure);               //��ʼ��PC.13
	/*������Դ*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;            //BT_PER->PB.3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	   //ͨ���������
  GPIO_Init(GPIOB, &GPIO_InitStructure); 
	GPIO_ResetBits(GPIOB,GPIO_Pin_3);	                   //��������Դ
	/*����־*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;            //CHRG_WKUP->PB.11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	       //��������
  GPIO_Init(GPIOB, &GPIO_InitStructure);               //��ʼ��PB.11
}

void Wkup_Init(void)
{
		SYSCLKConfig_STOP();  //���Ѳ�����HSE
		GPIO_Init_Wkup();
	  USART2_Init(115200);    //����2��ʼ��Ϊ115200
		USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//��������2�����ж�
	  MPU_Write_Byte(MPU_PWR_MGMT1_REG,0X01);  //ʹ��MPU6050
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
	
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;	           //BT_PWR-->PB.3 �˿�����
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;      //ͨ���������
   GPIO_Init(GPIOB, &GPIO_InitStructure);					       //�����趨������ʼ��GPIOB.3
	 GPIO_SetBits(GPIOB,GPIO_Pin_3);	                     //����ߣ������ر�
}
//ϵͳ����ͣ��ģʽ
void Sys_Enter_Stop(void)
{			 
	 RCC_APB2PeriphResetCmd(0X01FC,DISABLE);	//��λ����IO��
	 DisablePeriph();
	 DisableBT();
	 Sys_Stop();
}

/**
  * @brief  ͣ�����Ѻ�����ϵͳʱ��: ʹ�� HSE, PLL
  *         ����ѡ��PLL��Ϊϵͳʱ��.
  * @param  None
  * @retval None
  */
void SYSCLKConfig_STOP(void)
{
  /* After wake-up from STOP reconfigure the system clock */
  /* ʹ�� HSE */
  RCC_HSEConfig(RCC_HSE_ON);
  
  /* �ȴ� HSE ׼������ */
  while (RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET)
  {
  }
  
  /* ʹ�� PLL */ 
  RCC_PLLCmd(ENABLE);
  
  /* �ȴ� PLL ׼������ */
  while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
  {
  }
  
  /* ѡ��PLL��Ϊϵͳʱ��Դ */
  RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
  
  /* �ȴ�PLL��ѡ��Ϊϵͳʱ��Դ */
  while (RCC_GetSYSCLKSource() != 0x08)
  {
  }
}
