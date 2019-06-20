/*
* define.c



*/

#include "define.h"

//¿ØÖÆ×´Ì¬
bit F_CTRL_Boost = 0;
bit F_CTRL_OutputLeft = 0;
bit F_CTRL_OutputRight = 0;
bit F_CHG_END = 0;
xdata uint8_t cnt_VBCTRL_TIM = 0;

#define	C_CNT_VBCTRL_TIM_OPEN	2
#define	C_CNT_VBCTRL_TIM_CLOSE	8

void Open_Boost(void )
{
	CTRL_VB = 1;
	F_CTRL_Boost = 0;
	//cnt_VBCTRL_TIM = C_CNT_VBCTRL_TIM_OPEN;
	//CTRL_DET_OUT = 0;
	//F_CTRL_Boost = 1;
	
}

void Close_Boost(void )
{
	CTRL_VB = 0;
	F_CTRL_Boost = 0;
	//cnt_VBCTRL_TIM = C_CNT_VBCTRL_TIM_CLOSE;
	//CTRL_DET_OUT = 0;
	//F_CTRL_Boost = 0;
	
}
