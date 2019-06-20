#include "output.h"
#include "adc.h"
#include "bat.h"
#include "led.h"

#define C_Debounce_Max	100
#define C_Debounce_Key_Max	20

xdata uint8_t cnt_Debounce_KEY = 0;
xdata uint8_t cnt_Debounce_Door = 0;
xdata uint8_t cnt_Debounce_Input = 0;
xdata uint8_t cnt_Debounce_OutL = 0;
xdata uint8_t cnt_Debounce_OutR = 0;

bit DT_KEY_Before = 0;
bit DT_Hall_Before = 0;
bit DT_Input_Before = 0;
bit DT_OutL_Before = 0;
bit DT_OutR_Before = 0;


bit DT_KEY_Status = 0;
bit F_Cover_Status = 0;
bit F_Input_Status = 0;
bit F_OUTStatus_L = 0;
bit F_OUTStatus_R = 0;

bit F_Leftoutput_Stable = 0;
bit F_Rightoutput_Stable = 0;


#define C_DoorDisp_Close_Delay	5*4			//door open ,5s
#define C_DoorDisp_Open_Delay 	2*4			//door close, 2s
xdata uint8_t cnt_DoorDisp_Decnt = 0;
bit F_DoorDisp = 0;


bit KEY_Cont = 0;
bit KEY_Trg = 0;
bit KEY_Cont_S = 0;
xdata uint8_t cnt_DoubleClick = 0;
xdata uint8_t cnt_ContClick = 0;
#define C_CNT_Double_Delay	2		//0.5s
#define C_CNT_Cont_Delay	6		//3s

bit F_MSG_Key = 0;
bit F_KEY_DoubleClick = 0;
bit F_KEY_LongClick = 0;
xdata uint8_t cnt_delay_CoverChange = 0;
xdata uint8_t cnt_delay_Output = 0;
#define C_cnt_delay_CoverChange_Set 5*4		//5s
#define C_cnt_delay_Output_Set	1*4



#define C_decnt_Tim_MultiTrig_Set	300		//200ms for key trig
#define C_cnt_Key_LongPress_LL 10*4		//250ms*4*10
#define C_cnt_Key_LongPress_L 2*4		//250ms*4*10
#define C_cnt_Key_LongPress_Valid 2		//250ms*4*10


#define C_cnt_MuitiTrig_Max	3			//Max key trig
xdata uint16_t decnt_Tim_MultiTrig = 0;
xdata uint8_t cnt_Key_LongPress = 0;
xdata uint8_t cnt_MuitiTrig = 0;
xdata uint8_t MSG_KeyTrig = 0;
bit F_LongPress_Trig = 0;
xdata uint8_t cnt_Key_LongPress_Before = 0;


#define	C_decnt_Open_OutL_Set	2*4
#define	C_decnt_Open_OutR_Set	2*4

xdata uint8_t decnt_Open_OutR = 0;
xdata uint8_t decnt_Open_OutL = 0;
void check_Outside(void )
{
	//Door
	/**/
	if(DT_Hall == DT_Hall_Before)
	{
		if(cnt_Debounce_Door >= C_Debounce_Max)
		{
			//Get stable status
			if(F_Cover_Status == DT_Hall_Before)
			{
				//door action display
				F_DoorDisp = 1;
				//cnt_DoorDisp_Decnt = DT_Hall_Before? C_DoorDisp_Open_Delay:C_DoorDisp_Close_Delay;		
				//cnt_Disp_NewDevice = DT_Hall_Before? C_DoorDisp_Open_Delay:C_DoorDisp_Close_Delay;
				if(F_Cover_Status)
				{
					//open cover
					cnt_delay_CoverChange = C_cnt_delay_CoverChange_Set;
					
				}
				else
				{
					//close cover
					//cnt_delay_CoverChange = C_cnt_delay_CoverChange_Set;
					cnt_delay_Output = C_cnt_delay_Output_Set;
				}
				
				F_Cover_Status = !DT_Hall_Before;
			
			}
		}
		else
			cnt_Debounce_Door++;
			
	}
	else
	{		
		cnt_Debounce_Door = 0;
		DT_Hall_Before = DT_Hall;
	}
	
	//Key
	if(DT_KEY == DT_KEY_Before)
	{
		if(cnt_Debounce_KEY >= C_Debounce_Key_Max)
		{
			//Key status stable
			DT_KEY_Status = DT_KEY_Before;
			
			//Check multi key trig
			if(DT_KEY_Status && (!KEY_Cont))
			{
				//Key release
				if(F_LongPress_Trig)
				{
					//Long press release, ignore
					F_LongPress_Trig = 0;
				}
				else
				{
					cnt_MuitiTrig++;
					
					if(cnt_MuitiTrig == C_cnt_MuitiTrig_Max)
					{
						MSG_KeyTrig = cnt_MuitiTrig;
						F_MSG_Key = 1;
					}
					decnt_Tim_MultiTrig = C_decnt_Tim_MultiTrig_Set;
				}
			}
			else
			{
				//No trig, check over time
				if(decnt_Tim_MultiTrig == 0)
				{
					if((cnt_MuitiTrig > 0) && (cnt_MuitiTrig < C_cnt_MuitiTrig_Max))
					{
						MSG_KeyTrig = cnt_MuitiTrig;
						F_MSG_Key = 1;
					}
					cnt_MuitiTrig = 0;
				}
			}
			
			
			
			KEY_Cont = DT_KEY_Status;
			//check long press
			if(KEY_Cont == 0)
			{
				//Key down 
				if(cnt_Key_LongPress_Before != cnt_Key_LongPress)
				{
						
					if(cnt_Key_LongPress == C_cnt_Key_LongPress_LL)
					{
						F_MSG_Key = 1;
						MSG_KeyTrig = 5;
						printf("Key long long press trig\n");
					}
					else if(cnt_Key_LongPress == C_cnt_Key_LongPress_L)
					{
						F_MSG_Key = 1;
						MSG_KeyTrig = 4;
						printf("Key long press trig\n");
					}
					else if(cnt_Key_LongPress == C_cnt_Key_LongPress_Valid)
					{
						F_LongPress_Trig = 1;
					}
				}
				cnt_Key_LongPress_Before = cnt_Key_LongPress;
			}
			else
			{
				cnt_Key_LongPress = 0;
			}
			
			
			
//			
//			
//			if((!DT_KEY_Status)&&KEY_Cont)
//			{
//				//key trig，key press down
//				if(KEY_Trg == 1)
//				{
//					//double trig
//					KEY_Trg = 0;
//					F_KEY_DoubleClick = 1;
//				}
//				else
//				{
//					//first trig
//					KEY_Trg = 1;					//上升沿触发单击
//					cnt_DoubleClick = C_CNT_Double_Delay;
//					
//				}
//				cnt_Disp_NewDevice = 5*4;
//			}
//			if((KEY_Trg == 1) && (cnt_DoubleClick ==0))
//			{
//				//single click confirm
//				KEY_Trg = 0;
//				F_MSG_Key = 1;
//				LED_NewDevice_Set();
//			}
//			
//	
//			KEY_Cont = DT_KEY_Status;
//			if(KEY_Cont == 1)		//按键按下为高电平
//			{
//				//key down 
//				if(cnt_ContClick == 0 && (KEY_Cont_S == 0) )
//				{
//					//key continue down confirm
//					F_KEY_LongClick	 = 1;
//					KEY_Cont_S = 1;
//					
//				}
//			}
//			else
//			{
//				KEY_Cont_S = 0;
//				cnt_ContClick = C_CNT_Cont_Delay;
//			}
			
			
			
		}
		else
			cnt_Debounce_KEY++;
				
		
		
	}
	else
	{		
		cnt_Debounce_KEY = 0;
		DT_KEY_Before = DT_KEY;
	}
	
	
	
	//Input
	if(DT_Input == DT_Input_Before)
	{
		if(cnt_Debounce_Input >= C_Debounce_Max)
		{
			if(F_Input_Status == 0 && DT_Input_Before == 1)
			{
				//充电插入
				LED_NewDevice_Set();
			}
			
			if(F_Input_Status == 1 && DT_Input_Before == 0)
			{
				//充电移除
				//充电移除，5V会被关掉
				F_CTRL_Boost = 0;
				
			}
			F_Input_Status = DT_Input_Before;
			
		}
		else
			cnt_Debounce_Input++;
	}
	else
	{		
		cnt_Debounce_Input = 0;
		DT_Input_Before = DT_Input;
	}
	
	
	if(!F_CTRL_OutputLeft)
	{
		if(DT_OutL == DT_OutL_Before)
		{
			if(cnt_Debounce_OutL >= C_Debounce_Max)
			{
				if((DT_OutL_Before == 1) && (F_OUTStatus_L == 0))
				{
					//In trig
					decnt_Open_OutL = C_decnt_Open_OutL_Set;
				}
				F_OUTStatus_L = DT_OutL_Before;
				F_Leftoutput_Stable = 1;
			}
			else
				cnt_Debounce_OutL++;
		}
		else
		{		
			F_Leftoutput_Stable = 0;
			cnt_Debounce_OutL = 0;
			DT_OutL_Before = DT_OutL;
		}
	}
	
	
	if(!F_CTRL_OutputRight)
	{
		if(DT_OutR == DT_OutR_Before)
		{
			if(cnt_Debounce_OutR >= C_Debounce_Max)
			{
				if((DT_OutR_Before == 1) && (F_OUTStatus_R == 0))
				{
					//In trig
					decnt_Open_OutR = C_decnt_Open_OutR_Set;
				}
				F_OUTStatus_R = DT_OutR_Before;
				F_Rightoutput_Stable = 1;
			}
			else
				cnt_Debounce_OutR++;
		}
		else
		{		
			F_Rightoutput_Stable = 0;
			cnt_Debounce_OutR = 0;
			DT_OutR_Before = DT_OutR;
		}
	}
}


void boost_Handler(void)
{
	if(F_Input_Status)
	{
		Close_Boost();
	}
	else if(F_CTRL_OutputLeft || F_CTRL_OutputRight)
	{
		Open_Boost();
	}
	else
	{
		Close_Boost();
	}
}


/*
* 左边耳机输出控制
	输出开启状态下，判断输出电流是否过高或者过低，从而满电或者过流保护
	输出关闭状态下，判断输出是否存在，从而开启输出
20170620: 加入充电时不关闭输出
		充电未充满时不关闭输出
		但是满电显示依然存在，满电后电流大依然重新亮红灯			;逻辑要清晰
		


F_LeftOutput_Valid： 负载有效，检测到负载开启输出后电流超过10mA认为负载有效
Full：电流低于10mA并持续10s，认为满电

		充电中：
			输出一直打开（过流保护关闭）
			输出高于10mA认为负载有效，显示打开
			输出电流低于10mA，持续2s认为满电，满电指示灯，满电指示灯亮1min后熄灭
			
		不充电和充电一样
			输出电流低于10mA，持续2s认为满电
			持续10s关闭输出，等待1min关闭指示灯并休眠，
			

*/

bit F_Leftoutput_Valid = 0;
bit F_Rightoutput_Valid = 0;



bit F_Leftoutput_Full = 0;
bit F_Rightoutput_Full = 0;

//显示和控制完全单独处理，关联但是不交叉
bit F_Leftoutput_FullDisp = 0;
bit F_Rightoutput_FullDisp = 0;



xdata uint8_t Decnt_LeftOutput_FullDisp = 0;
xdata uint8_t Decnt_RightOutput_FullDisp = 0;

xdata uint8_t Decnt_Disp_LeftOutput_Full = 0;
xdata uint8_t Decnt_Disp_RightOutput_Full = 0;

xdata uint8_t Decnt_LeftOutput_Full = 0;
xdata uint8_t Decnt_RightOutput_Full = 0;


#define C_Decnt_Output_FullDisp_Set	3//2*4
#define C_Decnt_Output_Full_Set		10*4
#define C_Decnt_Disp_OutputFull_Set		240//60*4

void close_LeftOutput(void )
{
	CTRL_Out_L_OFF();
	F_Leftoutput_Stable = 0;		//Close output ,and outside will be indeterminate
	F_Leftoutput_Valid = 0;
	cnt_Debounce_OutL = 0;
}

void open_LeftOutput(void )
{
	CTRL_Out_L_ON();
	F_Leftoutput_Stable = 0;
	F_Leftoutput_Valid = 0;
}






void close_RightOutput(void )
{
	CTRL_Out_R_OFF();
	F_Rightoutput_Stable = 0;		//Close output ,and outside will be indeterminate
	F_Rightoutput_Valid = 0;
	cnt_Debounce_OutR = 0;
}

void open_RightOutput(void )
{
	CTRL_Out_R_ON();
	F_Rightoutput_Stable = 0;
	F_Rightoutput_Valid = 0;
}
#if 0
void leftOutput_Handler(void )
{
	if(F_Cover_Status)
	{
		if(F_CTRL_OutputLeft)
		{
			CTRL_Out_L_ON();
			if(F_Bat_Protect || F_LeftOutput_High)
			{
				close_LeftOutput();
				return;
			}
			
			if(F_LeftOutput_Low)
			{
				if(F_Leftoutput_Valid && (Decnt_LeftOutput_FullDisp == 0))
				{
					if(F_Leftoutput_FullDisp == 0)
						Decnt_Disp_LeftOutput_Full = C_Decnt_Disp_OutputFull_Set;
					F_Leftoutput_FullDisp = 1;
				}
				if(Decnt_LeftOutput_Full == 0)
					F_Leftoutput_Full = 1;
			}
			else
			{
				F_Leftoutput_FullDisp = 0;
				F_Leftoutput_Full = 0;
			}
			
			
			if(F_Leftoutput_Full)
			{
				F_Leftoutput_Valid = 0;
				if(F_Input_Status == 0)
					close_LeftOutput();
			}
		}
		else
		{
			CTRL_Out_L_OFF();
			if(!(F_Bat_Protect || F_Leftoutput_Full || F_LeftOutput_High))
			{
				if(cnt_delay_Output == 0)
					open_LeftOutput();
			}
			
		}
	}
	else
	{
		CTRL_Out_L_OFF();
		//load off
		F_Leftoutput_Valid = 0;
		F_Leftoutput_Full = 0;
		F_LeftOutput_High = 0;
		F_Leftoutput_FullDisp = 0;
		F_LeftOutput_Low = 0;
	}
}
void RightOutput_Handler(void )
{
	if(F_Cover_Status)
	{
		if(F_CTRL_OutputRight)
		{
			CTRL_Out_R_ON();
			if(F_Bat_Protect || F_RightOutput_High)
			{
				close_RightOutput();
				return;
			}
			
			if(F_RightOutput_Low)
			{
				if(F_Rightoutput_Valid && (Decnt_RightOutput_FullDisp == 0))
				{
					if(F_Rightoutput_FullDisp == 0)
						Decnt_Disp_RightOutput_Full = C_Decnt_Disp_OutputFull_Set;
					F_Rightoutput_FullDisp = 1;
				}
				if(Decnt_RightOutput_Full == 0)
					F_Rightoutput_Full = 1;
			}
			else
			{
				F_Rightoutput_FullDisp = 0;
				F_Rightoutput_Full = 0;
			}
			
			
			if(F_Rightoutput_Full)
			{
				F_Rightoutput_Valid = 0;
				if(F_Input_Status == 0)
					close_RightOutput();
			}
		}
		else
		{
			CTRL_Out_R_OFF();
			if(!(F_Bat_Protect || F_Rightoutput_Full || F_RightOutput_High))
			{
				if(cnt_delay_Output == 0)
					open_RightOutput();
			}
			
		}
	}
	else
	{
		CTRL_Out_R_OFF();
		//load off
		F_Rightoutput_Valid = 0;
		F_Rightoutput_Full = 0;
		F_RightOutput_High = 0;
		F_Rightoutput_FullDisp = 0;
		F_RightOutput_Low = 0;
	}
}
#else

void leftOutput_Handler(void )
{
	if(F_CTRL_OutputLeft)
	{
		CTRL_Out_L_ON();
		
		if(F_Bat_Protect || F_LeftOutput_High)
		{
			close_LeftOutput();
			return;
		}
		
		if(F_LeftOutput_Low)
		{
			if(F_Leftoutput_Valid && (Decnt_LeftOutput_FullDisp == 0))
			{
				if(F_Leftoutput_FullDisp == 0)
					Decnt_Disp_LeftOutput_Full = C_Decnt_Disp_OutputFull_Set;
				F_Leftoutput_FullDisp = 1;
			}
			if(Decnt_LeftOutput_Full == 0)
				F_Leftoutput_Full = 1;
		}
		else
		{
			F_Leftoutput_FullDisp = 0;
			F_Leftoutput_Full = 0;
		}
		
		
		if(F_Leftoutput_Full)
		{
			F_Leftoutput_Valid = 0;
			if(F_Input_Status == 0)
				close_LeftOutput();
			return;
		}
	}
	else
	{
		CTRL_Out_L_OFF();
		if(F_Leftoutput_Stable && !F_Bat_Protect)
		{
			if(F_OUTStatus_L)
			{
				//load exist
				if(F_LeftOutput_High || F_Leftoutput_Full || (decnt_Open_OutL >0))
					return;
				open_LeftOutput();
				
 			}
			else
			{
				//load off
				F_Leftoutput_Valid = 0;
				F_Leftoutput_Full = 0;
				F_LeftOutput_High = 0;
				F_Leftoutput_FullDisp = 0;
				F_LeftOutput_Low = 0;
			}
		}
		
	}
}

void rightOutput_Handler(void )
{
	if(F_CTRL_OutputRight)
	{
		CTRL_Out_R_ON();
		
		if(F_Bat_Protect || F_RightOutput_High)
		{
			close_RightOutput();
			return;
		}
		
		if(F_RightOutput_Low)
		{
			if(F_Rightoutput_Valid && (Decnt_RightOutput_FullDisp == 0))
			{
				if(F_Rightoutput_FullDisp == 0)
					Decnt_Disp_RightOutput_Full = C_Decnt_Disp_OutputFull_Set;
				F_Rightoutput_FullDisp = 1;
			}
			if(Decnt_RightOutput_Full == 0)
				F_Rightoutput_Full = 1;
		}
		else
		{
			F_Rightoutput_FullDisp = 0;
			F_Rightoutput_Full = 0;
		}
		
		
		if(F_Rightoutput_Full)
		{
			F_Rightoutput_Valid = 0;
			if(F_Input_Status == 0)
				close_RightOutput();
			return;
		}
	}
	else
	{
		CTRL_Out_R_OFF();
		if(F_Rightoutput_Stable && !F_Bat_Protect)
		{
			if(F_OUTStatus_R)
			{
				//load exist
				if(F_RightOutput_High || F_Rightoutput_Full || (decnt_Open_OutR > 0))
					return;
				open_RightOutput();
				
 			}
			else
			{
				//load off
				F_Rightoutput_Valid = 0;
				F_Rightoutput_Full = 0;
				F_RightOutput_High = 0;
				F_Rightoutput_FullDisp = 0;
				F_RightOutput_Low = 0;
			}
		}
		
	}
}
#endif


xdata uint8_t Offset_OutCur_High_Left = 0;
xdata uint8_t Offset_OutCur_Low_Left = 0;
xdata uint8_t Offset_OutCur_High_Right = 0;
xdata uint8_t Offset_OutCur_Low_Right = 0;
bit F_LeftOutput_High = 0;
bit F_LeftOutput_Low = 0;

bit F_RightOutput_High = 0;
bit F_RightOutput_Low = 0;

//uint16_t vol_Bat	=0;
//Check output current	
void check_OutPutCur(void )		
{
	//------------left---------------------
	//High
	if(current_Left >= OutCur_Left_High)
	{
		if(Offset_OutCur_High_Left >= C_Offset_OutCur_High_max)
		{
			Offset_OutCur_High_Left = (C_Offset_OutCur_High_max+C_Offset_OutCur_High_min)/2;
			F_LeftOutput_High = 1;			//Output high captured
		}
		else
			Offset_OutCur_High_Left++;	
	}
	else
	{
		if(Offset_OutCur_High_Left <= C_Offset_OutCur_High_min)
		{
			Offset_OutCur_High_Left = (C_Offset_OutCur_High_max+C_Offset_OutCur_High_min)/2;
			//F_LeftOutput_High = 0;
		}
		else
			Offset_OutCur_High_Left--;	
	}
	
	//Low
	if(current_Left <= OutCur_Left_Low)
	{
		if(Offset_OutCur_Low_Left >= C_Offset_OutCur_Low_max)
		{
			Offset_OutCur_Low_Left = (C_Offset_OutCur_Low_max+C_Offset_OutCur_Low_min)/2;
			if(F_LeftOutput_Low == 0)
			{
				
				F_LeftOutput_Low = 1;			//Output Low captured				
				Decnt_LeftOutput_Full = C_Decnt_Output_Full_Set;
				Decnt_LeftOutput_FullDisp = C_Decnt_Output_FullDisp_Set;
			}
		}
		else
			Offset_OutCur_Low_Left++;	
	}
	else
	{
		if(Offset_OutCur_Low_Left <= C_Offset_OutCur_Low_min)
		{
			Offset_OutCur_Low_Left = (C_Offset_OutCur_Low_max+C_Offset_OutCur_Low_min)/2;
			F_LeftOutput_Low = 0;
			F_Leftoutput_Valid = 1;
		}
		else
			Offset_OutCur_Low_Left--;	
	}
	
	//------------Right---------------------
	//High
	if(current_Right >= OutCur_Right_High)
	{
		if(Offset_OutCur_High_Right >= C_Offset_OutCur_High_max)
		{
			Offset_OutCur_High_Right = (C_Offset_OutCur_High_max+C_Offset_OutCur_High_min)/2;
			F_RightOutput_High = 1;			//Output high captured
		}
		else
			Offset_OutCur_High_Right++;	
	}
	else
	{
		if(Offset_OutCur_High_Right <= C_Offset_OutCur_High_min)
		{
			Offset_OutCur_High_Right = (C_Offset_OutCur_High_max+C_Offset_OutCur_High_min)/2;
			//F_RightOutput_High = 0;
		}
		else
			Offset_OutCur_High_Right--;	
	}
	
	//Low
	if(current_Right <= OutCur_Right_Low)
	{
		if(Offset_OutCur_Low_Right >= C_Offset_OutCur_Low_max)
		{
			Offset_OutCur_Low_Right = (C_Offset_OutCur_Low_max+C_Offset_OutCur_Low_min)/2;
			if(F_RightOutput_Low == 0)
			{
				
				F_RightOutput_Low = 1;			//Output Low captured				
				Decnt_RightOutput_Full = C_Decnt_Output_Full_Set;
				Decnt_RightOutput_FullDisp = C_Decnt_Output_FullDisp_Set;
			}
		}
		else
			Offset_OutCur_Low_Right++;	
	}
	else
	{
		if(Offset_OutCur_Low_Right <= C_Offset_OutCur_Low_min)
		{
			Offset_OutCur_Low_Right = (C_Offset_OutCur_Low_max+C_Offset_OutCur_Low_min)/2;
			F_RightOutput_Low = 0;
			F_Rightoutput_Valid = 1;
		}
		else
			Offset_OutCur_Low_Right--;	
	}
}