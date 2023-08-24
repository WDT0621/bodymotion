#include "stm32f10x.h"
#include "key.h"
#include "sys.h" 
#include "delay.h"
#include "led.h"

//������ʼ������
void KEY_Init(void) //IO��ʼ��
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//ʹ��PORTBʱ��

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_15;//BT_RES->PA.15
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
 	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.15

	//��ʼ�� KEY_ON-->GPIOB.10	  ��������
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PB10���ó����룬Ĭ������	  
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOB.10

	BKP_TamperPinCmd(DISABLE);//�ر����ּ�⹦�ܣ�Ҳ���� PC13��Ҳ���Ե���ͨIO ʹ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;            //INT
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	       //��������
  GPIO_Init(GPIOC, &GPIO_InitStructure);               //��ʼ��PC.13
}

u8 Check_WKUP(void) 
{
	u8 t=0;	//��¼���µ�ʱ��
	LED1 = 1; //�ر�LED
	while(1)
	{
		if(KEY_ON == 1)
		{
			t++;			//�Ѿ������� 
			delay_ms(30);
			if(t>=100)		//���³���3����
			{
				return 1; 	//����3s������
			}
		}else 
		{ 
			//LED2=0;
			//wakeup
			return 0; //���²���3��
		}
	}
}

u8 Check_Res(void) 
{
	u8 t=0;	//��¼���µ�ʱ��
	LED2 = 1;
	LED1 = 1; //�ر�LED
	while(1)
	{
		if(BT_RES == 0)
		{
			t++;			//�Ѿ������� 
			delay_ms(30);
			if(t>=100)		//���³���3����
			{
				return 1; 	//����3s������
			}
		}else 
		{ 
			return 0; //���²���3��
		}
	}
}