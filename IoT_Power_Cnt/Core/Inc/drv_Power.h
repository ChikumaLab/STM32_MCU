/*
 * drv_Power.h
 *
 *  Created on: 2023/11/08
 *      Author: User
 */

#ifndef INC_DRV_POWER_H_
#define INC_DRV_POWER_H_
#include "main.h"

extern void drv_Power_init(void);
extern float drv_Power_get_solarVolt(void);
extern float drv_Power_get_edlcVolt(void);
extern float drv_Power_get_mainVolt(void);
extern void drv_Power_set_edlc_dcdc(FunctionalState state);

#endif /* INC_DRV_POWER_H_ */
