/*
 * drv_ae_aqm1602a.h
 *
 *  Created on: Jul 23, 2022
 *      Author: User
 */

#ifndef INC_DRV_AE_AQM1602A_H_
#define INC_DRV_AE_AQM1602A_H_

#include "main.h"

extern uint8_t drv_ae_aqm1602a_write_cmd(uint8_t mode,uint8_t cmd);
extern void drv_ae_aqm1602a_init(I2C_HandleTypeDef *i2c);
extern void drv_ae_aqm1602a_test(void);
void drv_test_init(I2C_HandleTypeDef *i2c);


#endif /* INC_DRV_AE_AQM1602A_H_ */
