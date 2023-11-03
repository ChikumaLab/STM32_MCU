/*
 * app_lte_cmd.h
 *
 *  Created on: 2022/06/16
 *      Author: User
 */

#ifndef INC_APP_LTE_CMD_H_
#define INC_APP_LTE_CMD_H_

#include "main.h"

extern void    app_ltecmd_run(uint8_t *data,uint16_t len);
extern uint8_t app_set_mqtt_server(char *serverName,uint16_t port);
extern void    app_set_mqtt_sendData(char *topic,char *data);
extern uint8_t app_connect_mqtt_server(void);
extern uint8_t app_disconnect_mqtt_server(void);
extern uint8_t app_send_mqtt_data(void);

extern void send_lteCheckPwr_cmd(void);
extern void send_ltePwrOn_cmd(uint8_t onOff);
extern void app_lte_rxCmd_txPC(void);
extern void app_set_mqtt_sendData(char *topic,char *data);
extern uint8_t app_get_rssi(void);
extern uint8_t app_set_rf_param_init(void);

#endif /* INC_APP_LTE_CMD_H_ */
