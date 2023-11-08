/*
 * drv_cmd_If.h
 *
 *  Created on: 2022/06/05
 *      Author: User
 */

#ifndef INC_DRV_CMD_IF_H_
#define INC_DRV_CMD_IF_H_

#include "main.h"


#define DRV_UART_SYNC_DATA		(0x7E)
#define UART_PC_RX_MAX_LENGTH	(1024)

#define CLOUD_ADDRESS			(0x80)
#define PC_ADDRESS				(0x00)
#define MY_ADDRESS				(0x81)

typedef enum{
    UART_RX_STATE_SYNC = 0,
	UART_RX_STATE_LENGTH ,
	UART_RX_STATE_DATA
} DRV_UART_RX_STATE;

typedef enum
{
    SYS_BOOT = 0x00,
    SYS_MEM_READ ,
    SYS_MEM_WRITE,
    SYS_CPU_RESET,
    SYS_SET_RTC,
    SYS_GET_TIME,
    SYS_GET_FW_VER,
    SYS_SET_DB,
    SYS_GET_DB,
    SYS_SET_MODE,
    SYS_GET_MODE,

    PWR_CHECK_VOLT = 0x10,
    PWR_CHECK_PORT,
    PWR_SET_PORT,

}DRV_UART_CMD;

typedef enum
{
	// XBEE type
	CMD_AT_CMD_REQ = 0x08,
	CMD_AT_CMD_RESP = 0x88,
	CMD_MODEM_TYPE_IND = 0x8A,

	// My System type
    CMD_REQ = 0x30,
	CMD_RSP = 0x31,
	CMD_IND = 0x32,

	CMD_RMT_REQ = 0x33,
	CMD_RMT_RSP = 0x34,
	CMD_RMT_IND = 0x35,
}UART_CMD_TYPE;


extern uint8_t uart_pc_rxData[UART_PC_RX_MAX_LENGTH + 1];
extern DRV_UART_RX_STATE uart_pc_rxState;
extern uint8_t uart_pc_rxLength;
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart);
extern uint8_t uartTxData[2048];

extern void drv_pc_cmd_rx(void);
extern void drv_cmd_if_set_Uart(UART_HandleTypeDef *uart);
extern UART_HandleTypeDef *uart_pc_if;

extern uint16_t drv_pc_cmd_tx_sync(DRV_UART_CMD cmd,UART_CMD_TYPE cmdType,uint8_t *txData,uint16_t txBufLength);
extern void drv_pc_cmd_tx_ansync(DRV_UART_CMD cmd,UART_CMD_TYPE cmdType,uint8_t *txData,uint16_t txBufLength);


#endif /* INC_DRV_CMD_IF_H_ */
