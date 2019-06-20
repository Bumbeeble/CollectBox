#ifndef __I2C_SIMU_H
#define	__I2C_SIMU_H
//硬件模拟IIC总线读取EEPROM
#include "N76E003.h"
#include "SFR_Macro.h"
#include "Function_define.h"
#include "Common.h"
#include "define.h"
#include "delay.h"

#define I2C_DEBUG	1
typedef enum{FALSE = 0,TRUE = !FALSE} bool;

sbit	I2C_SCK = P1^3;
sbit	I2C_SDA = P1^4;
//引脚
//SDA: PB7;SCK: PB6
/*模拟IIC端口输出输入定义*/
#define I2C_SCK_H         do{I2C_SCK = 1;}while(0)//GPIO_SetBits(GPIOB,GPIO_Pin_6)
#define I2C_SCK_L         do{I2C_SCK = 0;}while(0)//GPIO_ResetBits(GPIOB,GPIO_Pin_6)
   
#define I2C_SDA_H         do{I2C_SDA = 1;}while(0)//GPIO_SetBits(GPIOB,GPIO_Pin_7)
#define I2C_SDA_L         do{I2C_SDA = 0;}while(0)//GPIO_ResetBits(GPIOB,GPIO_Pin_7)

#define I2C_SCL_read      I2C_SCK//GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6)
#define I2C_SDA_read      I2C_SDA//GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7)

void I2C_GPIOInit(void);
void I2C_delay(void);
bool I2C_Start(void);
void I2C_Stop(void);
void I2C_Ack(void);
void I2C_NoAck(void);
void I2C_WaitAck(void);
void I2C_SendByte(u8 SendByte);
unsigned char I2C_ReadByte(void);


#endif