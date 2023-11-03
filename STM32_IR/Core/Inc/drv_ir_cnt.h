/*
 * drv_ir_cnt.h
 *
 *  Created on: 2023/11/01
 *      Author: User
 */

#ifndef INC_DRV_IR_CNT_H_
#define INC_DRV_IR_CNT_H_

#include "main.h"
void ir_txInit(TIM_HandleTypeDef *htim,uint32_t timerCh,uint8_t mode);
void ir_dataTx(uint8_t *data,uint16_t len);
void drv_IR_int_task(void);


#endif /* INC_DRV_IR_CNT_H_ */
