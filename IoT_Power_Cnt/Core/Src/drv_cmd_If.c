/*
 * drv_cmd_If.c
 *
 *  Created on: 2022/06/05
 *      Author: User
 */

#include "drv_cmd_If.h"

UART_HandleTypeDef *uart_pc_if;
uint8_t uart_pc_rxData[UART_PC_RX_MAX_LENGTH + 1];
DRV_UART_RX_STATE uart_pc_rxState;
uint8_t uart_pc_rxLength;

void drv_cmd_if_set_Uart(UART_HandleTypeDef *uart)
{
	uart_pc_if = uart;
	uart_pc_rxState = UART_RX_STATE_SYNC;
	uart_pc_rxLength = 0;
	HAL_UART_Receive_IT(uart_pc_if, uart_pc_rxData, 1);

}


void drv_pc_cmd_rx(void)
{
	__IO uint8_t buf = 0;
	uint16_t i;
	switch(uart_pc_rxState)
	{
	case UART_RX_STATE_SYNC:
		if(uart_pc_rxData[0] == DRV_UART_SYNC_DATA)
		{
			uart_pc_rxState = UART_RX_STATE_LENGTH;
			HAL_UART_Receive_IT(uart_pc_if, uart_pc_rxData, 2);
			//HAL_UART_Receive_IT(uart_pc_if, &uart_pc_rxLength, 2);
		}
		else
		{
			uart_pc_rxState = UART_RX_STATE_SYNC;
			HAL_UART_Receive_IT(uart_pc_if, uart_pc_rxData, 1);
		}
		break;
	case UART_RX_STATE_LENGTH:
		uart_pc_rxLength = (uint16_t)(uart_pc_rxData[0]<<8) + (uint16_t)uart_pc_rxData[1];
		if(uart_pc_rxLength < UART_PC_RX_MAX_LENGTH)
		{
			if(uart_pc_rxLength != 0)
			{
				uart_pc_rxState = UART_RX_STATE_DATA;
				HAL_UART_Receive_IT(uart_pc_if, uart_pc_rxData, uart_pc_rxLength);
			}
			else
			{
				uart_pc_rxState = UART_RX_STATE_SYNC;
				HAL_UART_Receive_IT(uart_pc_if, uart_pc_rxData, 1);
			}
		}
		else
		{
			uart_pc_rxState = UART_RX_STATE_SYNC;
			HAL_UART_Receive_IT(uart_pc_if, uart_pc_rxData, 1);
		}
		break;
	case UART_RX_STATE_DATA:
		for(i = 0; i < uart_pc_rxLength - 1;i++)
		{
			buf += uart_pc_rxData[i];
		}
		buf = 0xFF - buf;
		if( uart_pc_rxData[i] == buf)
		{
			if(uart_pc_rxData[2] == MY_ADDRESS)
			{
				if(uart_pc_rxData[3] < 0x10)
				{
					app_syscmd_run(uart_pc_rxData,uart_pc_rxLength);
				}
				else
				{

				}
			}

		}
		else
		{

		}
		uart_pc_rxState = UART_RX_STATE_SYNC;
		HAL_UART_Receive_IT(uart_pc_if, uart_pc_rxData, 1);
		break;
	default:
		uart_pc_rxState = UART_RX_STATE_SYNC;
		HAL_UART_Receive_IT(uart_pc_if, uart_pc_rxData, 1);
		break;
	}
}


uint8_t uartTxData[2048];
uint16_t drv_pc_cmd_tx_sync(DRV_UART_CMD cmd,UART_CMD_TYPE cmdType,uint8_t *txData,uint16_t txBufLength)
{
    uint16_t length = 0;
    uint8_t dummy;
    uint8_t buf = 0;

    uartTxData[length++] = 0x7E;
    length = (uint16_t)(length + 2);

    uartTxData[length++] = cmdType;
    dummy = uartTxData[length - 1];

    uartTxData[length++] = MY_ADDRESS;
    dummy += uartTxData[length - 1];

    uartTxData[length++] = PC_ADDRESS;
    dummy += uartTxData[length - 1];

    uartTxData[length++] = (uint8_t)cmd;
    dummy += uartTxData[length - 1];

    for (int i = 0; i < txBufLength; i++)
    {
        uartTxData[length++] = txData[i];
        dummy += uartTxData[length - 1];
    }

    uartTxData[length++] = (uint8_t)(0xff - (dummy));
    buf = (uint8_t)(length - 3);

    uartTxData[1] = (uint8_t)((0xFF00 & buf) >> 8);
    uartTxData[2] = (uint8_t)((0x00FF & buf));

    HAL_UART_Transmit(uart_pc_if, uartTxData, length, 100);

    return length;
}

void drv_pc_cmd_tx_ansync(DRV_UART_CMD cmd,UART_CMD_TYPE cmdType,uint8_t *txData,uint16_t txBufLength)
{
    uint16_t length = 0;
    int dummy;
    uint8_t buf = 0;

    uartTxData[length++] = 0x7E;
    length = (uint16_t)(length + 2);

    uartTxData[length++] = cmdType;
    dummy = uartTxData[length - 1];

    uartTxData[length++] = MY_ADDRESS;
    dummy += uartTxData[length - 1];

    uartTxData[length++] = PC_ADDRESS;
    dummy += uartTxData[length - 1];

    uartTxData[length++] = (uint8_t)cmd;
    dummy += uartTxData[length - 1];

    for (int i = 0; i < txBufLength; i++)
    {
        uartTxData[length++] = txData[i];
        dummy += uartTxData[length - 1];
    }

    uartTxData[length++] = (uint8_t)(0xff - (dummy));
    buf = (uint8_t)(length - 3);

    uartTxData[1] = (uint8_t)((0xFF00 & buf) >> 8);
    uartTxData[2] = (uint8_t)((0x00FF & buf));

    HAL_UART_Transmit_IT(uart_pc_if, uartTxData, length);
}
