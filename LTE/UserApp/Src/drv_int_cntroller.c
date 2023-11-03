/*
 * drv_int_cntroller.c
 *
 *  Created on: 2022/10/05
 *      Author: User
 */
#include "drv_int_cntroller.h"

uint8_t lte_OnOffFlag  = 0;
uint8_t oled_view_mode = 1;
uint8_t timer_500ms_cnt = 0;

// UART RX Callback
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart[0].Instance == uart_pc_if[0].Instance)
	{
		drv_pc_cmd_rx();
	}
	else if(huart[0].Instance == lteModuleUart[0].Instance)
	{
		drv_lteModule_rxStart();
	}
	else if(huart[0].Instance == drv_gps_port_if[0].Instance)
	{
		drv_gps_rx_restart();
	}
	else if(huart[0].Instance == uart_xbee_if[0].Instance)
	{
		drv_xbee_cmd_rx();
	}
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
	if(huart[0].Instance == uart_pc_if[0].Instance)
	{
	}
	else if(huart[0].Instance == drv_gps_port_if[0].Instance)
	{
		drv_gps_rxError_restart();
	}
	else if(huart[0].Instance == lteModuleUart[0].Instance)
	{
		drv_lteModule_rxStart();
	}

}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim == lteModuleTimer)
    {
    	drv_ltModule_pwrOnTimer_stop();
    }
    if(htim == Timer_500ms)
    {
    	timer_500ms_cnt++;
    	if(timer_500ms_cnt % 20 == 0)
    	{
    	}
		if(oled_view_mode == 1)
		{
			app_add_onshot_Task (app_oled_view_temp);
		}
		else if(oled_view_mode == 2)
		{
			app_add_onshot_Task (app_oled_view_lte);
		}
		else if(oled_view_mode == 0)
		{
			app_add_onshot_Task (drv_oled_ssd1306_clear);
		}
		if(timer_500ms_cnt > 30)
		{
			timer_500ms_cnt = 0;
			app_add_onshot_Task (app_mqtt_send_tempData);
			app_add_onshot_Task (app_mqtt_send_gpsData);
		}
    	HAL_TIM_Base_Start_IT(Timer_500ms);
    }
}


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(lte_OnOffFlag == 0)
	{
		lte_OnOffFlag = 1;
		app_add_onshot_Task (app_lte_StartStop);
	}
}
