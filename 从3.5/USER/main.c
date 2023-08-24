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
        {} ; //�����ݻ�û�б�ת�Ƶ���λ�Ĵ�����ȴ�
					USART2->DR=s[i];//����һ���ֽڵ�����
      }
}
void Send_datatoU1(int length ,u8 *s)
{
			int i=0;
      for(i=0;i<length;i++)
      {
				while((USART1->SR&0x0080)==0)
        {} ; //�����ݻ�û�б�ת�Ƶ���λ�Ĵ�����ȴ�
					USART1->DR=s[i];//����һ���ֽڵ�����
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
	//u8 Reset[3]={0x02,0xC0,0x06};						//�ָ���������
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
    
	  
	  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	  USART1_Init(115200);    //����1��ʼ��
		USART2_Init(115200);    //����2��ʼ��
  	delay_init();	       //��ʱ��ʼ�� 
		LED_Init();		  			//��ʼ����LED���ӵ�Ӳ���ӿ�
	  KEY_Init();
	  Adc_Init();
  	EXTIX_Init();
	  BTPwrSet();
		MPU_Init();					//��ʼ��MPU6050
    mpu_dmp_init();
	  RTC_Init();
	  LED1 = 0;	//LED��	
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
		USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//��������2�����ж�
		PWR_EN = 1;
		StopFlag = 0;
		SleepFlag = 0;
		delay_ms(500);
		TIM3_Int_Init(29999,7199);//10Khz�ļ���Ƶ�ʣ�������30000Ϊ3s 	
		LED1 = 0;	//LED��
		STMFLASH_Read(FLASH_SAVE_ADDR,(u16*)flashdata,11);
		flashdata;
		
		//BTRestore();
		
		while(1)		//��ѭ��
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
							TIM_Cmd(TIM3, DISABLE);  //�ر�TIMx
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
                   LED1 = 0;         //������
								   LED2 = 1;
						    }
						  else{
							  LED1 = 0;            //���׶�1
								LED2 = 0;
						  }
			      }
						else{
							 LED2 = 0;            //���׶�0
						}
			   }
					else{
						  if(ADCtemp < LOWPWR){
								if(ADCtemp < Very_LOWPWR){
									PWR_EN = 0;
								}
								else{
								LED1 = 1;
							 delay_ms(80); LED2 = 0;	 	//LED��
		           delay_ms(80); LED2 = 1;		 	//LED��
		           delay_ms(80); LED2 = 0;	 	//LED��
		           LED2 = 1;										//LED��
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
				 
				 if(USART_GetFlagStatus(USART2, USART_FLAG_RXNE) != RESET)//�����ݼĴ����ǿ�
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
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//��������2�����ж�
	while(1){}
}
