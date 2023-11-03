/*
 * drv_int_cntroller.h
 *
 *  Created on: 2022/10/05
 *      Author: User
 */

#ifndef INC_DRV_INT_CNTROLLER_H_
#define INC_DRV_INT_CNTROLLER_H_

#include "main.h"

extern void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
extern void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
extern void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart);

#endif /* INC_DRV_INT_CNTROLLER_H_ */
