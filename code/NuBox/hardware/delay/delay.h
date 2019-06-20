#ifndef     _DELAY_H_
#define     _DELAY_H_
#include "N76E003.h"
#include "SFR_Macro.h"
#include "Function_define.h"
#include "Common.h"
/**/
void Delay(uint32_t nCount);
#define	Delay_us(x) Delay(x)
#define osDelay(x) Delay(x)

#endif


