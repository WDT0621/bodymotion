#ifndef __WKUP_H
#define __WKUP_H	 
#include "sys.h"
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
					    
void Sys_Stop(void);	 
void Sys_Enter_Stop(void);	//ϵͳ����ͣ��ģʽ
void SYSCLKConfig_STOP(void);   //ͣ�����Ѻ�����ϵͳʱ��: ʹ�� HSE, PLL
void Wkup_Init(void);
void GPIO_Init_Wkup(void);
#endif


