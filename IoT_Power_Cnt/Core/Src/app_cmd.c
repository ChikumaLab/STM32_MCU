/*
 * app_cmd.c
 *
 *  Created on: Nov 3, 2023
 *      Author: s20-0
 */

#include "app_cmd.h"

void send_sysCpcReset_cmd(void);


void app_syscmd_run(uint8_t *data,uint16_t len)
{
	uint16_t cmd;
	cmd = data[3];

	switch(cmd)
	{
	case SYS_MEM_READ:
		break;
	case SYS_MEM_WRITE:
		break;
	case SYS_CPU_RESET:
		send_sysCpcReset_cmd();
		break;
	case SYS_SET_RTC:
		break;
	case SYS_GET_TIME:	// DataGet all
		break;
	case SYS_GET_FW_VER:	// GyroDataGet
		break;
	case SYS_SET_DB:	// AcceleDataGet
		break;
	case SYS_GET_DB:	// MagDataGet
		break;
	case SYS_SET_MODE:	// MagDataGet
		break;
	case SYS_GET_MODE:	// MagDataGet
		break;
	default:
		break;
	}

}



void app_pwrcmd_run(uint8_t *data,uint16_t len)
{
	uint8_t cmd;
	cmd = data[3];

	switch(cmd)
	{
	case PWR_CHECK_VOLT:
		break;
	case PWR_CHECK_PORT:
		break;
	case PWR_SET_PORT:
		break;
	default:
		break;
    }

}

void app_per_wet_cmd(uint8_t *data)
{
    if(data[0] == 0x00)
    {
        
    }

}


void send_sysBoot_cmd(void)
{
	drv_pc_cmd_tx_sync(SYS_BOOT,CMD_IND,NULL,0);
}


void send_sysCpcReset_cmd(void)
{
	HAL_NVIC_SystemReset();
}
