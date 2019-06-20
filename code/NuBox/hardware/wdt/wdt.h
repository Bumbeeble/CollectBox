#ifndef		_WDT_H_
#define		_WDT_H_
#include "N76E003.h"
#include "SFR_Macro.h"
#include "Function_define.h"
#include "Common.h"


#define		WDTPS_6_4ms			0
#define		WDTPS_25_6ms		1
#define		WDTPS_51_2ms		2
#define		WDTPS_102_4ms		3
#define		WDTPS_204_8ms		4
#define		WDTPS_409_6ms		5
#define		WDTPS_819_2ms		6
#define		WDTPS_1638ms		7


void Enable_WDT_Reset_Config(void);
void WDT_config(uint8_t preScalar);
#define		WDT_Clear()	do{BIT_TMP=EA;EA=0;TA=0xAA;TA=0x55;WDCON|=SET_BIT6;EA=BIT_TMP;}while(0)
#define		WDT_Stop()	do{BIT_TMP=EA;EA=0;TA=0xAA;TA=0x55;WDCON&=~SET_BIT7;EA=BIT_TMP;}while(0)
#endif



