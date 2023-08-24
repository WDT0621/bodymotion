#include "led.h"
    
//LED IO��ʼ��
void LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOA, ENABLE);	 //ʹ��PA��PB�˿�ʱ��
	
	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOF, ENABLE); 
 GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;				     //LED1-->PB.6 �˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 		 //��©���
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);					       //�����趨������ʼ��GPIOB.6
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;	           //LED2-->PB.7 �˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;     //��©���
 GPIO_Init(GPIOB, &GPIO_InitStructure);					       //�����趨������ʼ��GPIOB.7
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;	           //PWR_EN-->PA.6 �˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
 GPIO_Init(GPIOA, &GPIO_InitStructure);					       //�����趨������ʼ��GPIOA.6
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;				     //CHRG_WKUP-->PB.11 �˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 		     //��������
 GPIO_Init(GPIOB, &GPIO_InitStructure);					       //�����趨������ʼ��GPIOB.11
 
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;				     //BT_PWR-->PB.3 �˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		 //
 GPIO_Init(GPIOB, &GPIO_InitStructure);					       //�����趨������ʼ��GPIOB.11
	
 GPIO_SetBits(GPIOB,GPIO_Pin_6);						           //PC.6 ����� LED1��
 GPIO_SetBits(GPIOB,GPIO_Pin_7);						           //PC.7 ����� LED2��
 GPIO_ResetBits(GPIOB,GPIO_Pin_3);						         //PB.3 ����� BTʹ��
 GPIO_ResetBits(GPIOB,GPIO_Pin_10);						         //PC.10 ����� PWR��ʹ��

}
 
