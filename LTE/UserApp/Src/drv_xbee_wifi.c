/*
 * drv_xbee_wifi.c
 *
 *  Created on: 2022/11/05
 *      Author: User
 */
#include "drv_xbee_wifi.h"

DRV_UART_RX_STATE uart_xbee_rxState;
uint8_t uart_xbee_rxData[UART_XBEE_RX_MAX_LENGTH + 1];
uint8_t uart_xbee_txData[UART_XBEE_RX_MAX_LENGTH + 1];
uint8_t uart_xbee_rxLength;
uint8_t drv_xbee_cmd_tx(uint8_t *data,uint16_t len);

void drv_xbee_set_uartPort(UART_HandleTypeDef *uart)
{
	uart_xbee_if = uart;
	uart_xbee_rxState = UART_RX_STATE_SYNC;
	uart_xbee_rxLength = 0;
	HAL_UART_Receive_IT(uart_xbee_if, uart_xbee_rxData, 1);

}
void drv_xbee_set_ap_name(uint8_t *data,uint8_t len)
{

}
void drv_xbee_set_ap_pass(uint8_t *data,uint8_t len)
{

}

void drv_xbee_reset(void)
{
	HAL_GPIO_WritePin(XBEE_RESET_GPIO_Port, XBEE_RESET_Pin, GPIO_PIN_RESET);
	HAL_Delay(100);
	HAL_GPIO_WritePin(XBEE_RESET_GPIO_Port, XBEE_RESET_Pin, GPIO_PIN_SET);}

void drv_xbee_getReq_rssi(void)
{
	uint8_t cmd[] = {0x08,0x5A,0x4C,0x4D,0x04};

	drv_xbee_cmd_tx(cmd,sizeof(cmd) - 1);

}

uint8_t drv_xbee_cmd_tx(uint8_t *data,uint16_t len)
{
	uart_xbee_txData[0] = DRV_UART_SYNC_DATA;

	uart_xbee_txData[1] = (uint8_t)((0xFF00 & len) >> 8);
	uart_xbee_txData[2] = (uint8_t)((0x00FF & len));
	uart_xbee_rxState = UART_RX_STATE_SYNC;
	uart_xbee_rxLength = 0;
	HAL_UART_Receive_IT(uart_xbee_if, uart_xbee_rxData, 1);

	return 0;

}


void drv_xbee_cmd_rx(void)
{
	__IO uint8_t buf = 0;
	uint16_t i;
	switch(uart_xbee_rxState)
	{
	case UART_RX_STATE_SYNC:
		if(uart_xbee_rxData[0] == DRV_UART_SYNC_DATA)
		{
			uart_xbee_rxState = UART_RX_STATE_LENGTH;
			HAL_UART_Receive_IT(uart_xbee_if, uart_xbee_rxData, 2);
			//HAL_UART_Receive_IT(uart_pc_if, &uart_pc_rxLength, 2);
		}
		else
		{
			uart_xbee_rxState = UART_RX_STATE_SYNC;
			HAL_UART_Receive_IT(uart_xbee_if, uart_xbee_rxData, 1);
		}
		break;
	case UART_RX_STATE_LENGTH:
		uart_xbee_rxLength = (uint16_t)(uart_xbee_rxData[0]<<8) + (uint16_t)uart_xbee_rxData[1] + 1;
		if(uart_xbee_rxLength < UART_XBEE_RX_MAX_LENGTH)
		{
			if(uart_xbee_rxLength != 0)
			{
				uart_xbee_rxState = UART_RX_STATE_DATA;
				HAL_UART_Receive_IT(uart_xbee_if, uart_xbee_rxData, uart_xbee_rxLength);
			}
			else
			{
				uart_xbee_rxState = UART_RX_STATE_SYNC;
				HAL_UART_Receive_IT(uart_xbee_if, uart_xbee_rxData, 1);
			}
		}
		else
		{
			uart_xbee_rxState = UART_RX_STATE_SYNC;
			HAL_UART_Receive_IT(uart_xbee_if, uart_xbee_rxData, 1);
		}
		break;
	case UART_RX_STATE_DATA:
		for(i = 0; i < uart_xbee_rxLength - 1;i++)
		{
			buf += uart_xbee_rxData[i];
		}
		buf = 0xFF - buf;
		if( uart_xbee_rxData[i] == buf)
		{
			switch(uart_xbee_rxData[0])
			{
			case CMD_AT_CMD_REQ:
				break;
			case CMD_AT_CMD_RESP:
				break;
			case CMD_MODEM_TYPE_IND:
				break;
			default:
				break;
			}

		}
		else
		{

		}
		uart_xbee_rxState = UART_RX_STATE_SYNC;
		HAL_UART_Receive_IT(uart_xbee_if, uart_xbee_rxData, 1);
		break;
	default:
		uart_xbee_rxState = UART_RX_STATE_SYNC;
		HAL_UART_Receive_IT(uart_xbee_if, uart_xbee_rxData, 1);
		break;
	}
}
