#ifndef _ADC_H_
#define _ADC_H_

#include "N76E003.h"
#include "SFR_Macro.h"
#include "Function_define.h"
#include "Common.h"
#define		NUM_ADChannel	6

#define MAX_ADCCycle    10				//第一级最多10次采样

#define	SET_ADC_OFFSET_MAX	30
#define	SET_ADC_OFFSET_MIN	2

typedef	enum
{
	ADC_Channel_0 = 0,
	ADC_Channel_1 = 1,
	ADC_Channel_2 = 2,
	ADC_Channel_3 = 3,
	ADC_Channel_4 = 4,
	ADC_Channel_5 = 5,
	ADC_Channel_6 = 6,
	ADC_Channel_7 = 7,
	ADC_Channel_8 = 8
}	type_adc_channel;

struct ADC_Type
{
	type_adc_channel  channel;			//通道设定
	uint16_t Data;			//采样值
	uint8_t sign;
	uint16_t value;			//采样值
	
	uint16_t offset;
};
extern idata  struct ADC_Type ADC_Data[NUM_ADChannel];
extern xdata  uint8_t F_ADC_Over ;
extern uint16_t err_cnt;
extern xdata uint8_t BandgapHigh,BandgapLow,BandgapMark;
extern bit	F_TIM_ADCValue;
extern xdata uint16_t Bandgap_Value, Bandgap_Voltage;
extern bit	F_Ready_ADCData;
extern xdata  uint16_t current_Left;
extern xdata  uint16_t current_Right;
extern xdata  uint16_t vol_Bat;

void ADC_Config(void);
void Read_ADC(void );
void Get_ADCValue(void );
void ADC_Cycle_Start(void );
void READ_BANDGAP();
void ADC_Deconfig(void);


#endif

