#ifndef __sys_tim_H
#define __sys_tim_H
#include "N76E003.h"
#include "Function_define.h"
#include "Common.h"
#include "SFR_Macro.h"
#include "define.h"
#include "bat.h"
#include "output.h"
#include "adc.h"

#include "output.h"
//系统时钟分频，16MHz内部时钟，0：不分频； 1：2倍的CKDIV分频
#define C_ClockDiv_SET		0

#if C_ClockDiv_SET
	#define	C_TIM0_Reload		(65535-800/C_ClockDiv_SET)		//100us
#else
	#define	C_TIM0_Reload		(65535-1600)		//100us
#endif

extern xdata uint8_t main_Flag_100ms;
extern xdata uint8_t sys_tim_Flag_1ms;
extern xdata uint8_t sys_tim_Flag_250ms;
extern xdata uint8_t sys_tim_Flag_100ms;
extern xdata uint8_t sys_tim_Flag_1s;


extern bit F_LED_Blink;

void sys_tim_Deinit(void);
void sys_tim_Init(void );
void systick_Handler(void );
#endif




