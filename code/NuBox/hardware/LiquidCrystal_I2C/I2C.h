#ifndef	_I2C_H_
#define	_I2C_H_

#include "N76E003.h"
#include "SFR_Macro.h"
#include "Function_define.h"
#include "Common.h"
#include "delay.h"
#define I2C_Direct_WR               0
#define I2C_Direct_RD               1

void init_I2C1(void);
void I2C_StartTransmission(uint8_t Address, uint8_t Direction);
void I2C_WriteData( uint8_t send_data);
uint8_t I2C_ReadData(uint8_t ack_status);
void I2C_Stop(void);
#endif




