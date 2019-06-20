#ifndef _OUTPUT_H_
#define _OUTPUT_H_
#include "define.h"

#define OutCur_Left_High	0x85//0x7A				//150mA/0x7A; 100mA/0x51
#define OutCur_Left_Low		0x03				//10mA/0x08

#define OutCur_Right_High	0x85//0x7A				//150mA/0x7A; 100mA/0x51
#define OutCur_Right_Low	0x03				//10mA/0x08

#define C_Offset_OutCur_High_max	15
#define C_Offset_OutCur_High_min	5

#define C_Offset_OutCur_Low_max	100
#define C_Offset_OutCur_Low_min	5


extern bit F_Cover_Status;
extern bit F_Input_Status;
extern bit F_OUTStatus_L;
extern bit F_OUTStatus_R;
extern bit F_Leftoutput_Stable;
extern bit F_Rightoutput_Stable;

extern bit F_LeftOutput_High;
extern bit F_LeftOutput_Low;

extern bit F_RightOutput_High;
extern bit F_RightOutput_Low;

extern bit F_CTRL_OutputLeft;
extern bit F_CTRL_OutputRight;
extern bit F_CTRL_Boost;

extern bit F_Leftoutput_Full;
extern bit F_Rightoutput_Full;
extern bit F_DoorDisp;
//显示和控制完全单独处理，关联但是不交叉
extern bit F_Leftoutput_FullDisp;
extern bit F_Rightoutput_FullDisp;


extern bit F_Leftoutput_Valid;
extern bit F_Rightoutput_Valid;

extern xdata uint8_t Decnt_LeftOutput_FullDisp;
extern xdata uint8_t Decnt_RightOutput_FullDisp;

extern xdata uint8_t Decnt_Disp_LeftOutput_Full;
extern xdata uint8_t Decnt_Disp_RightOutput_Full;

extern xdata uint8_t Decnt_LeftOutput_Full;
extern xdata uint8_t Decnt_RightOutput_Full;

extern xdata uint8_t cnt_DoubleClick;
extern xdata uint8_t cnt_ContClick;

extern bit F_MSG_Key;
extern bit F_KEY_DoubleClick;
extern bit F_KEY_LongClick;

extern xdata uint8_t cnt_delay_CoverChange;
extern xdata uint8_t cnt_delay_Output;

extern xdata uint16_t decnt_Tim_MultiTrig;
extern xdata uint8_t cnt_Key_LongPress;
extern xdata uint8_t cnt_MuitiTrig;
extern xdata uint8_t MSG_KeyTrig;
extern bit F_LongPress_Trig;

extern xdata uint8_t decnt_Open_OutR;
extern xdata uint8_t decnt_Open_OutL;
//Check output current	
void check_OutPutCur(void );
void check_Outside(void );
void leftOutput_Handler(void );
void rightOutput_Handler(void );
void boost_Handler(void);
#endif