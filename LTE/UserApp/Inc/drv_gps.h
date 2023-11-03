/*
 * drv_gpis.h
 *
 *  Created on: 2022/08/22
 *      Author: User
 */

#ifndef INC_DRV_GPS_H_
#define INC_DRV_GPS_H_

#include "main.h"

#define DRV_GPS_RX_MAX_BUF_SIZE	(1024)

extern UART_HandleTypeDef *drv_gps_port_if;
extern uint8_t drv_gps_data_rxBuf[DRV_GPS_RX_MAX_BUF_SIZE];

extern void drv_gps_init(UART_HandleTypeDef *port);
extern void drv_gps_rx_restart(void);
extern uint16_t drv_gps_get_data(uint8_t *rxData);
extern uint8_t drv_gps_culc_checksum(uint8_t *data,uint16_t len);
extern void app_gps_rxCmd_txPC(void);
extern void drv_gps_rxError_restart(void);
extern void app_gps_rxData_WriteFIle(void);

extern uint8_t drv_gps_time_sync_flag;


#endif /* INC_DRV_GPS_H_ */
