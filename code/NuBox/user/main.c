//-------------chip----------------------
#include "N76E003.h"
#include "SFR_Macro.h"
#include "Function_define.h"
#include "Common.h"

#include "stdlib.h"
//-------------system----------------------
#include "sys_tim.h"
//#include "sys_init.h"
#include "wdt.h"
//-------------hardware----------------------
#include "led.h"
#include "adc.h"
//#include "output.h"
//#include "bat.h"
//#include "sleep.h"
//-------------UART---------------------
//#include  "halfduplex.h"
#include "uart.h"
#include "LiquidCrystal_I2C.h"
#include  "buzzer.h"
#include "button.h"
#include "encoder.h"

bit BIT_TMP;

void key_msg_Handler(void );


//void EAR_Handler(void )
//{
//	if(F_Cover_Status)
//	{
//		//close, power off
//		BASE_R_Low();
//		BASE_L_Low();
//	}
//	else
//	{
//		//open cover ,delay 1s then power on
//		if(cnt_delay_Output == 0)
//		{
//			BASE_R_High();
//			BASE_L_High();
//		}
//	}
//}
uint16_t cnt_TIM = 0;

char char_buf[20] = {0};
int16_t buf_enc = 0;
//The main C function
void main (void) 
{
	
	sys_tim_Init();					//????
	Enable_WDT_Reset_Config();			//??CONFIG4,??WDT
	WDT_config(WDTPS_1638ms);			//WDT????1.638s
	LED_Init();						//LED
	Buzzer_Init();
	Button_Init();
	Encoder_Init();
	//GPIO_Init();
	ADC_Config();
	ADC_Cycle_Start();				//ADC
	UART_Init(100000);		//9600
//	cnt_delay_CoverChange = 2*4;
//	Decnt_SleepDelay = C_SleepDelay_Set;
	set_EA;                        //enable interrupts
	I2C_GPIOInit();
	LCDI2C_init(0x4E,16,2);	// 0x27
	printf("I2C LCD Init OK");
	LCDI2C_setCursor(0,0);
	LCDI2C_write_String("Program Start...");
	printf("System start...\n");
	while(1)
	{
		//
		WDT_Clear();
		if(main_Flag_100ms)
		{
			main_Flag_100ms = 0;
			sprintf(char_buf, "0%04dmv %04dmv",ADC_Data[0].value,ADC_Data[0].Data);// ((uint32_t)ADC_Data[0].Data) * 3300/4096, ((uint32_t)ADC_Data[1].Data) * 3300/4096);
			if(ADC_Data[0].sign)
			{
				char_buf[0] = '+';
			}
			else
			{
				char_buf[0] = '-';
			}
			LCDI2C_setCursor(0,0);
			LCDI2C_write_String(char_buf);
			sprintf(char_buf, " Encoder: %04d", buf_enc);
			LCDI2C_setCursor(0,1);
			LCDI2C_write_String(char_buf);
		}
	}
}
	
void Mainloop_Hander_Int(void )
{
	Read_ADC();	
	Get_ADCValue();	
	if(sys_tim_Flag_1ms)
	{
		sys_tim_Flag_1ms = 0;
		//??1ms????
		Read_Button();
		if(Get_Button_Trg(0))
		{
			Clear_Button_Trg(0);
			Set_Buz0(1 , 0);
			Clr_EncoderData();
		}
		if(buf_enc != Get_EncoderData())
		{
			//Set_Buz0(1,0);
			buf_enc = Get_EncoderData();
			
		}
	}
	if(sys_tim_Flag_100ms)
	{
		//
		sys_tim_Flag_100ms = 0;
		Buzzer_Handler();
		
		
	}
	if(sys_tim_Flag_1s)
	{
		sys_tim_Flag_1s = 0;
			
		LED_R_RUN();
		LED_G_RUN();
		cnt_TIM++;
	}
}
void key_msg_Handler(void )
{
	
}