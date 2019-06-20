#ifndef		__UART0_H_
#define		__UART0_H_


#include "N76E003.h"
#include "SFR_Macro.h"
#include "Function_define.h"
#include "Common.h"
#include "define.h"
#include "stdio.h"
#define Bumbee_DEBUG 1

// head*4 + cmd*2 + len*2 + data*len + checksum
#define	RX_Head_LEN	4

#define	RX_CMD_LEN 3
//position of the length, 
#define	RX_LEN_POS	(RX_Head_LEN + RX_CMD_LEN)
#define	RX_DATA_LEN_MAX	100
typedef struct {
	uint8_t rx_over;
	uint8_t rx_cnt;
	uint8_t rx_data[RX_DATA_LEN_MAX];
	uint8_t rx_data_len;
	uint8_t rx_TM_Decnt;
	uint8_t err;
	
}tUART_RecvStruct;
#define C_rx_OverTM_Max	30

extern xdata uint8_t decnt_recvbuf;
extern xdata tUART_RecvStruct RecvBuf_UART0;
//-------------------------IO setup-------------------------
sbit	UART_TX_L = P0^6;
sbit	UART_RX_L = P0^7;
sbit	UART_TX_R = P1^6;
sbit	UART_RX_R = P0^2;



//-------------------------var def-------------------------

//-------------------------macro declaration-------------------------
#define UART0_Interrupt_ON()	do{ES = 1;}while(0)
#define UART0_Interrupt_OFF()	do{ES = 0;}while(0)

#define UART0_Receive_ON()		do{REN = 1;}while(0)
#define UART0_Receive_OFF()		do{REN = 0;}while(0)

#define UART1_Interrupt_ON()	do{set_ES_1;}while(0)
#define UART1_Interrupt_OFF()	do{clr_ES_1;}while(0)

#define UART1_Receive_ON()		do{REN_1 = 1;}while(0)
#define UART1_Receive_OFF()		do{REN_1 = 0;}while(0)

//---------------function declaration-------------------------
void UART_Deinit(void );
void UART_Init(uint32_t baudrate);
void  Send_Data_To_UART0(uint8_t c);
uint8_t Receive_Data_From_UART0(void);
void  Send_Data_To_UART1(uint8_t c);
uint8_t Receive_Data_From_UART1(void);

void set_UART0_TXMode(void );
void set_UART0_RXMode(void );

void set_UART1_TXMode(void );
void set_UART1_RXMode(void );


void UART0_OverTM_Handler(void );
void UART0_RxHandler(void );
void UART0_Rx_Clean(void );
#endif




