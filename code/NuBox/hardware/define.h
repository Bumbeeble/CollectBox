#ifndef	_DEFINR_H_
#define	_DEFINR_H_

#include "N76E003.h"
#include "SFR_Macro.h"
#include "Function_define.h"
#include "Common.h"


typedef	uint8_t u8;
typedef	uint16_t u16;
typedef	uint32_t u32;

/*
*---------------------------------------
* 常量定义
*/
#define	C_TCC_Reload		6		//207	//TCC定时，1ms
//#define C_TIM_CNT_1ms		20		
#define	C_TIM_CNT_250ms		250		//250ms定时
#define C_TIM_CNT_1min		240
#define C_TIM_CNT_1hour		60
/*
*---------------------------------------
* 变量声明
*/
extern bit F_TCC_Int ;
extern bit F_TIM_1ms;
extern bit F_ADC_AllOver;
extern bit F_CHG_END;
//输入检测引脚定义
sbit DT_Input	 = P1^2;
#define DT_Input_config()		do{P12_Input_Mode;}while(0)

sbit DT_OutL	 = P1^5;
#define DT_OutL_config()		do{P15_Input_Mode;}while(0)

sbit DT_OutR	 = P1^0;
#define DT_OutR_config()		do{P10_Input_Mode;}while(0)

sbit DT_CHG		 = P1^7;
#define DT_CHG_config()			do{P17_Input_Mode;}while(0)
	
sbit DT_KEY		 = P1^3;
#define DT_KEY_config()			do{DT_KEY = 1;P13_Quasi_Mode;}while(0)

sbit DT_Hall	 = P1^1;
#define DT_Hall_config()		do{P11_Input_Mode;}while(0)	

//控制状态
//输出控制引脚定义
sbit CTRL_Out_L	 = P0^0;
#define CTRL_Out_L_config()		do{P00_PushPull_Mode;}while(0)
sbit CTRL_Out_R	 = P0^4;
#define CTRL_Out_R_config()		do{P04_PushPull_Mode;}while(0)

sbit CTRL_VB	 = P3^0;
#define CTRL_VB_config()		do{P30_PushPull_Mode;}while(0)


sbit BASE_TX_L	 = P0^6;
#define BASE_TX_L_config()		do{P06_Quasi_Mode;}while(0)

sbit BASE_RX_L	 = P0^7;
#define BASE_RX_L_config()		do{P07_Quasi_Mode;}while(0)

sbit BASE_TX_R	 = P1^6;
#define BASE_TX_R_config()		do{P16_Quasi_Mode;}while(0)	

sbit BASE_RX_R	 = P0^2;
#define BASE_RX_R_config()		do{P02_Quasi_Mode;}while(0)

#define BASE_L_High()		do{BASE_TX_L = 1; }while(0)
#define BASE_L_Low()		do{BASE_TX_L = 0; }while(0)

#define BASE_R_High()		do{BASE_TX_R = 1; }while(0)
#define BASE_R_Low()		do{BASE_TX_R = 0; }while(0)


//sbit CTRL_DET_OUT	= P1^1;
//#define CTRL_DET_OUT_config()	do{P11_PushPull_Mode;}while(0)
	

extern bit F_CTRL_Boost;
extern bit F_CTRL_OutputLeft;
extern bit F_CTRL_OutputRight;
extern xdata uint8_t cnt_VBCTRL_TIM;
#define CTRL_Out_L_ON()		CTRL_Out_L = 1;F_CTRL_OutputLeft = 1
#define CTRL_Out_L_OFF()	CTRL_Out_L = 0;F_CTRL_OutputLeft = 0

#define CTRL_Out_R_ON()		CTRL_Out_R = 1;F_CTRL_OutputRight = 1
#define CTRL_Out_R_OFF()	CTRL_Out_R = 0;F_CTRL_OutputRight = 0

#define CTRL_DET_OUT_ON()	CTRL_DET_OUT = 1
#define CTRL_DET_OUT_OFF()	CTRL_DET_OUT = 0

#define	C_CNT_VBCTRL_TIM_OPEN	2
#define	C_CNT_VBCTRL_TIM_CLOSE	8
void Open_Boost(void );
void Close_Boost(void );

/*
//输入检测引脚定义
#define DT_Hall		P60
#define DT_Input	P61
#define DT_OutL		P62//P51
#define DT_OutR		P63//P50
#define DT_CHG		P51//P57
#define DT_KEY		P72

//输出控制引脚定义
#define CTRL_Out_L	P55
#define CTRL_Out_R	P54
#define CTRL_DET_OUT		P50

*/

#endif