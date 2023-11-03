/*
 * drv_lteModule.h
 *
 *  Created on: 2022/06/04
 *      Author: User
 */

#ifndef INC_DRV_LTEMODULE_H_
#define INC_DRV_LTEMODULE_H_

#include "main.h"

extern void drv_lteModule_set_timer(TIM_HandleTypeDef *timer);
extern void drv_lteModule_rxStart(void);
extern void drv_lteModule_InitSeq(UART_HandleTypeDef *uart);
extern void drv_lteModule_startSeq(void);
extern void drv_lteModule_stopSeq(void);
extern uint8_t drv_lteModule_get_rxdPort_State(void);
extern void drv_ltModule_pon_cnt(void);
extern void drv_lteModule_bufferEn(uint8_t enable);
extern uint8_t drv_ltModule_getV_int(void);
extern void drv_ltModule_pwrOnTimer_start(void);
extern void drv_ltModule_pwrOnTimer_stop(void);
extern void drv_ltModule_send_AtCmd(char *data,uint16_t len);
extern void drv_ltModule_Send_PC_responce(void);
extern uint8_t drv_ltModule_cmd_respWait(char *rxData,char *checkStr);
extern void drv_ltModule_ucged_read(void);

extern UART_HandleTypeDef *lteModuleUart;
extern TIM_HandleTypeDef *lteModuleTimer;

#endif /* INC_DRV_LTEMODULE_H_ */
