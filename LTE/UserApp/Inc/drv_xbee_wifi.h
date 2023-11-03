/*
 * drv_xbee_wifi.h
 *
 *  Created on: 2022/11/05
 *      Author: User
 */

#ifndef INC_DRV_XBEE_WIFI_H_
#define INC_DRV_XBEE_WIFI_H_

#include "main.h"
#define UART_XBEE_RX_MAX_LENGTH	(1024)

extern void drv_xbee_set_uartPort(UART_HandleTypeDef *uart);
extern void drv_xbee_cmd_rx(void);
extern void drv_xbee_reset(void);

extern UART_HandleTypeDef huart4;

#endif /* INC_DRV_XBEE_WIFI_H_ */
