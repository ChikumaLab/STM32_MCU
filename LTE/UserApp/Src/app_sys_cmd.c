/*
 * app_sys_cmd.c
 *
 *  Created on: 2022/06/12
 *      Author: User
 */
#include "app_sys_cmd.h"

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

void send_sysBoot_cmd(void)
{
	drv_pc_cmd_tx_sync(SYS_BOOT,CMD_IND,NULL,0);
}


void send_sysCpcReset_cmd(void)
{
	HAL_NVIC_SystemReset();
}

void sed_rtc_set_cmd()
{
	  RTC_TimeTypeDef sTime = {0};
	  RTC_DateTypeDef sDate = {0};

	  sTime.Hours = 0x0;
	  sTime.Minutes = 0x0;
	  sTime.Seconds = 0x0;
	  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
	  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
	  {
	    Error_Handler();
	  }
	  sDate.WeekDay = RTC_WEEKDAY_WEDNESDAY;
	  sDate.Month = RTC_MONTH_JANUARY;
	  sDate.Date = 0x1;
	  sDate.Year = 0x20;

	  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
	  {
	    Error_Handler();
	  }
}
