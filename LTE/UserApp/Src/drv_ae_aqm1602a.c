/*
 * drv_ae_aqm1602a.c
 *
 *  Created on: Jul 23, 2022
 *      Author: User
 */

#include "drv_ae_aqm1602a.h"
#define AE_AQM1602A_I2C_ADDR	(0x7C)
I2C_HandleTypeDef *i2c_ae_aqm1602a_if;

#define CMD_AE_AQM1602A_SET_CMD			(0x00)
#define CMD_AE_AQM1602A_SET_DATA		(0x01)
uint8_t ae_aqm1602a_txData[2];




void drv_ae_aqm1602a_init(I2C_HandleTypeDef *i2c)
{
	i2c_ae_aqm1602a_if = i2c;
	HAL_Delay(100);

	drv_ae_aqm1602a_write_cmd(CMD_AE_AQM1602A_SET_CMD,0x38);
	HAL_Delay(1);

	drv_ae_aqm1602a_write_cmd(CMD_AE_AQM1602A_SET_CMD,0x39);
	HAL_Delay(1);

	drv_ae_aqm1602a_write_cmd(CMD_AE_AQM1602A_SET_CMD,0x14);
	HAL_Delay(1);

	drv_ae_aqm1602a_write_cmd(CMD_AE_AQM1602A_SET_CMD,0x7F);
	HAL_Delay(1);

	drv_ae_aqm1602a_write_cmd(CMD_AE_AQM1602A_SET_CMD,0x56);
	HAL_Delay(1);

	drv_ae_aqm1602a_write_cmd(CMD_AE_AQM1602A_SET_CMD,0x6C);
	HAL_Delay(300);

	drv_ae_aqm1602a_write_cmd(CMD_AE_AQM1602A_SET_CMD,0x38);
	HAL_Delay(1);

	drv_ae_aqm1602a_write_cmd(CMD_AE_AQM1602A_SET_CMD,0x01);
	HAL_Delay(1);

	drv_ae_aqm1602a_write_cmd(CMD_AE_AQM1602A_SET_CMD,0x0C);
	HAL_Delay(1);
}

uint8_t drv_ae_aqm1602a_write_cmd(uint8_t mode,uint8_t cmd)
{
	uint8_t ret = 0;
	if(mode == 0)
	{
		ae_aqm1602a_txData[0] = 0x00;
	}
	else
	{
		ae_aqm1602a_txData[0] = 0x40;
	}
	ae_aqm1602a_txData[1] = cmd;

	if(i2c_ae_aqm1602a_if[0].State != HAL_I2C_STATE_READY)
	{
		ret = 0xFF;
	}
	else
	{
		HAL_I2C_Master_Transmit_IT(i2c_ae_aqm1602a_if, AE_AQM1602A_I2C_ADDR, ae_aqm1602a_txData, sizeof(ae_aqm1602a_txData));
	}

	return ret;
}
char moji[] ="AQM1602XA-RN-GBW";

void drv_ae_aqm1602a_test(void)
{
	for(int i = 0; i < 16; i++)
	{
		drv_ae_aqm1602a_write_cmd(CMD_AE_AQM1602A_SET_DATA,moji[i]);
		HAL_Delay(1);
	}
	drv_ae_aqm1602a_write_cmd(CMD_AE_AQM1602A_SET_CMD,0x40+0x80);
	HAL_Delay(1);
	 for(int i = 0; i < 16; i++)
	 {
			drv_ae_aqm1602a_write_cmd(CMD_AE_AQM1602A_SET_DATA,moji[i]);
			HAL_Delay(1);
	 }
}
