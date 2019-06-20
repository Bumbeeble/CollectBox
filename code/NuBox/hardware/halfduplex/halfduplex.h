#ifndef		__HALFDUPLEX_H_
#define		__HALFDUPLEX_H_

#include "uart.h"
#include "sys_tim.h"
#include "led.h"

#define LEN_TX_BUF 50
//Head*4 + CMD*2 + LENGTH*2 + Data*LENGTH + checksum*1
#define LENGTH_UART_TX_HEAD	4
#define LENGTH_TX_CMD 2
#define LENGTH_TX_DATA_MAX	(LEN_TX_BUF - LENGTH_UART_TX_HEAD - LENGTH_TX_CMD - 1)

extern code uint8_t uart_Tx_CMD[][2];

typedef enum{
	TX_CMD_CHECKSTATE = 0,
	TX_CMD_KEY_1_Trig,
	TX_CMD_KEY_2_Trig,
	TX_CMD_KEY_3_Trig,
	TX_CMD_KEY_L_Press,
	TX_CMD_KEY_LL_Press,
}TX_CMD;

typedef enum {
	UART_TX_MODE_CHECKSTATE = 0,
	UART_TX_MODE_KEY,
	UART_TX_MODE_Pair,
	UART_TX_MODE_Close,
}UART_TX_MODE;

typedef enum{
	UART_TX_STATUS_Free = 0,
	UART_TX_STATUS_Receiver,
	
}UART_TX_STATUS;
/**/
typedef struct {
	UART_TX_MODE mode;			//current mode
	UART_TX_MODE mode_request;			//request for mode switch after current transmit
	TX_CMD cmd_request;
	
	
	UART_TX_STATUS tx_status;		//transmit status,0: free; 1:transmit; 2: receiver
	
	uint8_t result_rx;		//
	uint16_t decnt_rx;			//decounter for receiver after transmit
	
	uint8_t cnt_err;			//err counter, to much err will raise no connect handler
	uint8_t flag_err;			//current error status
	uint8_t buf[LEN_TX_BUF];	//data buf
}t_Struct_UART_TxCMD;


//transmint data by interrupt struct
typedef struct {
	uint8_t tx_status;
	uint8_t *tx_buf;
	uint8_t tx_pos;
	uint8_t tx_len;
}t_Struct_UART_TxBuf;

extern xdata t_Struct_UART_TxBuf Struct_TxBuf_UART0;

#ifdef	UART_Need_Delay
extern xdata uint8_t delay_uart;
#endif


int8_t uart0_TxBuf(uint8_t *databuf, uint8_t length);
void send_String_UART1(uint8_t str[], uint8_t len);

int8_t uart_sendCMD(TX_CMD tx_cmd, uint8_t tx_data[], uint8_t length_tx_data );
void uart0_CMDHandler(void );

void uart0_ModeHandler_Checkstate(void );
void uart0_ModeHandler_Key(void );
int8_t uart0_CMD_Request(UART_TX_MODE cmd_mode, TX_CMD cmd_msg );
#endif


