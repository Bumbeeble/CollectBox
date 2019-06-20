/*
* sys_init.c

*/


#include "sys_init.h"
void GPIO_Init(void )
{
	DT_KEY_config();
	DT_Input_config();//{P04_Input_Mode;}while(0)
	DT_OutL_config();//{P03_Input_Mode;}while(0)
	DT_OutR_config();//{P15_Input_Mode;}while(0)
	DT_CHG_config();//{P13_Input_Mode;}while(0)

	CTRL_Out_L_config();//{P17_Output_Mode;}while(0)
	CTRL_Out_R_config();//{P14_Output_Mode;}while(0)
	
	CTRL_Out_L_OFF();
	CTRL_Out_R_OFF();
//	
//	BASE_TX_L_config();
//	BASE_RX_L_config();
//	
//	BASE_TX_R_config();
//	BASE_RX_R_config();
	
}




