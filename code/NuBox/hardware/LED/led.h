/*
* LEDœ‘ æ
	LED0: PC13
	
*/

#ifndef __LED_H
#define __LED_H


#include "N76E003.h"
#include "Function_define.h"
#include "Common.h"

#include "SFR_Macro.h"
#include "output.h"

sbit	LED_R = P1^2;
#define		LED_R_OUT()	do{P12_PushPull_Mode;}while(0)
sbit	LED_G = P1^5;
#define		LED_G_OUT()	do{P15_PushPull_Mode;}while(0)

#define		LED_R(x)	do{LED_R = x;}while(0)
#define		LED_G(x)	do{LED_G = x;}while(0)


#define		LED_R_ON()	do{LED_R(0);}while(0)
#define		LED_G_ON()	do{LED_G(0);}while(0)

#define		LED_R_OFF()	do{LED_R(1);}while(0)
#define		LED_G_OFF()	do{LED_G(1);}while(0)

#define		LED_R_RUN()	do{LED_R = !LED_R;}while(0)
#define		LED_G_RUN()	do{LED_G = !LED_G;}while(0)



void LED_Init(void);
#endif

