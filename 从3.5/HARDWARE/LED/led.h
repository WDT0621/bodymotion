#ifndef __LED_H
#define __LED_H	 
#include "sys.h"

#define LED1 PBout(6)
#define LED2 PBout(7)
#define PWR_EN PAout(6)
#define CHRG PBin(11)
#define BT_PWR PBout(3)

void LED_Init(void);//≥ı ºªØ

		 				    
#endif
