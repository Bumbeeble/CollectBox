#ifndef _BUTTON_H_
#define _BUTTON_H_
#include "N76E003.h"
#include "SFR_Macro.h"
#include "Function_define.h"
#include "Common.h"
//#include "type_define.h"
typedef enum {OFF = 0, ON = !OFF} Status;
#define C_Button_Debounce_Max	20
sbit button0 = P2^0;
#define _Read_Button0()	button0//GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0)
#define _Read_Button1()	1//GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1)

#define Button0	0
#define Button1	1


void Button_Init(void);
void Read_Button(void);
Status Get_Button_Trg(uint8_t );
void Clear_Button_Trg(uint8_t );
Status Get_Button_Status(uint8_t );
Status Get_Button_Continue(uint8_t pos);



#endif
