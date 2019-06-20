#include "sleep.h"
#include "output.h"
#include "adc.h"
#include "bat.h"
#include "uart.h"
#include "sys_tim.h"
#include "wdt.h"
//#include "init.h"
uint8_t Decnt_SleepDelay = 0;

void PIO_Interrupt()	 interrupt 7     //ISR for io change interrupt
{
	PIF = 0;
}
void sleep_Handler(void )
{
	if(	((F_Input_Status || F_CTRL_Boost) == 0 ) && F_Leftoutput_Stable && F_Rightoutput_Stable && (cnt_delay_CoverChange == 0))
	{
		if(Decnt_SleepDelay == 0)
		{
			//sleep confirm 
			ADC_Deconfig();
			UART_Deinit();
			sys_tim_Deinit();
			
			PICON = 0xFD;		//select Port 1 wake up, all pin
			PINEN = 0x2F;		//Enable falling edge interrupt£¬0B00101111
			PIPEN = 0x2F;		//Enable raising edge interrupt
			PIF = 0;
			EIE |= (0x01<<1);
			clr_BODEN;
			EA = 1;
			set_PD;
			
			//wake up 
			WDT_Clear();
			EIE &= ~(0x01<<1);
			PICON = 0xFD;		//select Port 1 wake up, all pin
			PINEN = 0x00;		//Enable falling edge interrupt£¬0B00101111
			PIPEN = 0x00;		//Enable raising edge interrupt
			PIF = 0;
			
			sys_tim_Init();					//????
			ADC_Config();
			ADC_Cycle_Start();				//ADC
			UART_Init(100000);		//9600
			
			Decnt_SleepDelay = C_SleepDelay_Set;
		}
	}
	else
	{
		Decnt_SleepDelay = C_SleepDelay_Set;
	}
	
}