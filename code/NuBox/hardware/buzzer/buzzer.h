/*
* 蜂鸣器
	
*/

#ifndef __BUZZER_H
#define __BUZZER_H
#include "N76E003.h"
#include "SFR_Macro.h"
#include "Function_define.h"
#include "Common.h"

sbit		IO_BUZ = P0^1;
#define 	IO_BUZ_OUT()	do{P01_PushPull_Mode;}while(0)
#define 	IO_BUZ_RUN()	do{IO_BUZ = !IO_BUZ;}while(0)
#define 	IO_BUZ_OFF()	do{IO_BUZ = 0;}while(0)

extern bit	F_BUZ_Status;
void Buzzer_Init(void);
void Buzzer_Handler(void);			//100ms运行一次，处理蜂鸣器任务
void  Set_Buz0(uint8_t Buz_CNT, uint8_t Buz_Type);				//设置蜂鸣器
#endif
