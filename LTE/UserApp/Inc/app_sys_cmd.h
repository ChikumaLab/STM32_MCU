/*
 * app_sys_cmd.h
 *
 *  Created on: 2022/06/12
 *      Author: User
 */

#ifndef INC_APP_SYS_CMD_H_
#define INC_APP_SYS_CMD_H_

#include "main.h"

extern void send_sysBoot_cmd(void);
extern void app_syscmd_run(uint8_t *data,uint16_t len);
extern void send_sysCpcReset_cmd(void);


#endif /* INC_APP_SYS_CMD_H_ */
