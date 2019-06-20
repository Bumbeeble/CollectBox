/*
* LED.c

*/

#include "led.h"
#include "sys_tim.h"
#include "bat.h"
void LED_Scan( void);
void LED_StatusSet(void );

xdata uint8_t cnt_LED_UART = 0;

#define		C_CNT_Disp_NewDevice_Set	2
xdata uint8_t cnt_Disp_NewDevice = 0;

uint8_t pos_led = 0;
uint8_t LED_Status = 0;			//LED◊¥Ã¨…Ë∂®£¨µÕ4bit¡¡√£¨∏ﬂ4bit…¡À∏
bit F_TIM_LEDRUN = 0;

void LED_Init()
{

	LED_R(0);
	LED_G(0);
	LED_R_OUT();
	LED_G_OUT();
}



