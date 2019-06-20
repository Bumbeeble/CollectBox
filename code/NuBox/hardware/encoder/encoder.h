#ifndef _ENCODER_H_
#define _ENCODER_H_
#include "N76E003.h"
#include "SFR_Macro.h"
#include "Function_define.h"
#include "Common.h"

#include "button.h"
sbit	ENCODER_A = P0^0;
sbit	ENCODER_B = P1^0;

#define	_Encoder_Config()	do{P00_Quasi_Mode;P10_Quasi_Mode;}while(0)
#define _Read_EncodeA()	ENCODER_A//GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5)
#define _Read_EncodeB()	ENCODER_B//GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6)

#define EncodeA	2
#define EncodeB	3
void Clr_EncoderData(void );
int16_t Get_EncoderData(void );
void Set_EncoderData(int16_t data_Set );

void Read_Encoder(void);
void Encoder_Init(void);

#endif
