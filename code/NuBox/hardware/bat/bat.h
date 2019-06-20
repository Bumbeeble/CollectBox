#ifndef _BAT_H_
#define _BAT_H_
#include "define.h"

#define	C_BatLevel_SetMax	6

extern bit F_vol_Bat_State;
extern bit F_Bat_Protect;
extern bit F_Bat_Full;
extern xdata uint8_t Decnt_BatFull;
extern xdata uint8_t bat_level;
extern xdata uint8_t cnt_debounce_bat_level;
void Check_BatLevel(void );
void check_BatVol(void );
void bat_Handler(void );
#endif
