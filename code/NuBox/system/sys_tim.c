#include "sys_tim.h"
#include  "buzzer.h"
#include "adc.h"
#include "sleep.h"
#include "uart.h"
#include "encoder.h"
extern void Mainloop_Hander_Int(void );
static xdata uint8_t sys_tim_CNT_1ms = 0;
static xdata uint8_t sys_tim_CNT_250ms = 0;
static xdata uint8_t sys_tim_CNT_100ms = 0;
static xdata uint8_t sys_tim_CNT_1s = 0;

xdata uint8_t sys_tim_Flag_1ms = 0;
xdata uint8_t sys_tim_Flag_250ms = 0;
xdata uint8_t sys_tim_Flag_100ms = 0;
xdata uint8_t main_Flag_100ms = 0;
xdata uint8_t sys_tim_Flag_1s = 0;

#define	C_TIM_LED_Blink	500
static xdata uint16_t cnt_LED_Blink = 0;
bit F_LED_Blink = 0;

//xdata uint16_t tim_cal = 0;
//xdata uint16_t cnt_cal = 0;
bit F_systick = 0;
xdata uint8_t cnt_DT_CHG = 0;
//time base handler
void systick_Handler()
{
	//---------------1ms----------------------------------------
	if(F_systick == 0)
		return;
	F_systick = 0;
	sys_tim_Flag_1ms = 1;
//#ifdef	UART_Need_Delay
//	if(delay_uart > 0)
//		delay_uart--;
//#endif
//	if(decnt_recvbuf > 0)
//	{
//		decnt_recvbuf --;
//	}
//	if(cnt_LED_UART > 0)
//	{
//		cnt_LED_UART--;
//	}
//	if(decnt_Tim_MultiTrig > 0)
//	{
//		decnt_Tim_MultiTrig --;
//	}
//	//LED
//	cnt_LED_Blink++;
//	if(cnt_LED_Blink >= C_TIM_LED_Blink)
//	{
//		cnt_LED_Blink = 0;
//		F_LED_Blink = !F_LED_Blink;
//		//F_LED_Blink = 1;
//	}
//	F_TIM_LEDRUN = 1;
//	F_TIM_ADCValue = 1;
//	
	sys_tim_CNT_100ms++;
	if(sys_tim_CNT_100ms >= 100)
	{
		sys_tim_CNT_100ms = 0;
		sys_tim_Flag_100ms = 1;
		main_Flag_100ms = 1;
	}
	
//	if(DT_CHG)
//	{
//		cnt_DT_CHG ++;
//	}
		
	//-------------------------------------------------
	sys_tim_CNT_250ms++;
	if(sys_tim_CNT_250ms>=250)
	{
		sys_tim_CNT_250ms = 0;
		sys_tim_Flag_250ms = 1;
		//-----------------250ms-------------------------
//		if(cnt_delay_CoverChange > 0)
//			cnt_delay_CoverChange--;
//		
//		if(cnt_delay_Output > 0)
//			cnt_delay_Output--;
//		
//		if(cnt_Disp_NewDevice > 0)
//			cnt_Disp_NewDevice--;
//		//250ms Task
//		//if(cnt_DoorDisp_Decnt)
//		//	cnt_DoorDisp_Decnt--;
//			
//		if(Decnt_LeftOutput_Full)
//			Decnt_LeftOutput_Full--;
//			
//		if(Decnt_LeftOutput_FullDisp)
//			Decnt_LeftOutput_FullDisp--;
//			
//		if(Decnt_Disp_LeftOutput_Full)
//			Decnt_Disp_LeftOutput_Full--;
//		else
//			F_Leftoutput_FullDisp = 0;
//			
//		if(Decnt_RightOutput_Full)
//			Decnt_RightOutput_Full--;
//			
//		if(Decnt_RightOutput_FullDisp)
//			Decnt_RightOutput_FullDisp--;
//		
//		if(Decnt_Disp_RightOutput_Full)
//			Decnt_Disp_RightOutput_Full--;
//		else
//			F_Rightoutput_FullDisp = 0;
//				
//		if(Decnt_BatFull)
//			Decnt_BatFull--;
//		if(Decnt_SleepDelay)
//			Decnt_SleepDelay--;
//		
//		if(cnt_ContClick)
//			cnt_ContClick--;
//		
//		
//		if(cnt_DoubleClick)
//			cnt_DoubleClick--;
//		
//		//Battery level debounce
//		if(cnt_debounce_bat_level > 0)
//			cnt_debounce_bat_level --;
//		
//		
//		//Charge end detect
//		if(cnt_DT_CHG > 250/3)
//		{
//			F_CHG_END = 1;
//		}
//		else
//		{
//			F_CHG_END = 0;
//		}
//		cnt_DT_CHG = 0;
//		
//		if(decnt_Open_OutR > 0)
//		{
//			decnt_Open_OutR --;
//		}
//		
//		if(decnt_Open_OutL > 0)
//		{
//			decnt_Open_OutL --;
//		}
		sys_tim_CNT_1s++;
		if(sys_tim_CNT_1s >= 4)
		{
			sys_tim_CNT_1s = 0;
			sys_tim_Flag_1s = 1;
		}
//		
//		if(cnt_Key_LongPress < 100)
//		{
//			cnt_Key_LongPress ++;
//		}
		/*cnt_TIM_1min++;
		if(cnt_TIM_1min < C_TIM_CNT_1min)
			return;
	//----------Reach 1min----------------------
		cnt_TIM_1min = 0;
		F_TIM_1min = 1;
		cnt_TIM_1hour++;
		if(cnt_TIM_1hour < C_TIM_CNT_1hour)
			return;
	//----------Reach 1hour----------------------
		cnt_TIM_1hour = 0;
		F_TIM_1hour = 1;
		*/
	}
}
#define c_cnt_systick_Set 10
xdata uint8_t cnt_systick = 0;

#define c_cnt_BUZ_RUN_Set 2
xdata uint8_t cnt_BUZ_RUN = 0;
/**
  * @brief  This function handles sys_tim Handler.
  * @param  None
  * @retval None
	* 100us 
  */
void sys_tim_Interrupt(void)  interrupt 1  //interrupt address is 0x000B
{
	//100us
	Read_Encoder();
	TH0 += C_TIM0_Reload/256;
    TL0 += C_TIM0_Reload%256;
	cnt_systick++;
	if(cnt_systick >= c_cnt_systick_Set)
	{
		//1ms
		cnt_systick = 0;
		F_systick = 1;
	}
	
	cnt_BUZ_RUN++;
	if(cnt_BUZ_RUN >= c_cnt_BUZ_RUN_Set)
	{
		cnt_BUZ_RUN = 0;
		if(F_BUZ_Status)
		{
			IO_BUZ_RUN();
		}
		else
		{
			IO_BUZ_OFF();
		}
	}
	systick_Handler();
	Mainloop_Hander_Int();
	
	
}

void sys_tim_Deinit(void)
{
	clr_ET0;
    clr_TR0;
}
void sys_tim_Init(void)
{
	//system clock division
	//CKDIV = C_ClockDiv_SET;				//系统时钟16M，1/4分频，4MHz主频
	
	
	//TIMER0_MODE1_ENABLE;
	TMOD&=0xF0;
	TMOD|=0x01;
	set_T0M;					//选择不分频，默认为51默认的12分频
    TH0 = C_TIM0_Reload/256;
    TL0 = C_TIM0_Reload%256; 
    set_ET0;
    set_TR0;
}


