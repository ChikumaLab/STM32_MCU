/*
 * drv_Power.c
 *
 *  Created on: 2023/11/08
 *      Author: User
 */


#include "drv_Power.h"

#define DRV_POWER_ADC_REF_VOLT		(3.3)
#define DRV_POWER_ADC_RESOLUTION	(4096.6)
#define DRV_POWER_ADC_VOLT_DIV		(2)
#define DRV_POWER_EDLC_POS			(0)
#define DRV_POWER_SOLAR_POS			(1)
#define DRV_POWER_MAIN_POS			(2)

extern ADC_HandleTypeDef hadc1;
static uint16_t adcData[3];


void drv_Power_init(void)
{
	HAL_ADC_Start_DMA(&hadc1,(uint32_t *)adcData,3);
}

float drv_Power_get_solarVolt(void)
{
	return ((DRV_POWER_ADC_REF_VOLT / DRV_POWER_ADC_RESOLUTION) *
			(float)adcData[DRV_POWER_SOLAR_POS]) * DRV_POWER_ADC_VOLT_DIV ;
}

float drv_Power_get_edlcVolt(void)
{
	return ((DRV_POWER_ADC_REF_VOLT / DRV_POWER_ADC_RESOLUTION) *
			(float)adcData[DRV_POWER_EDLC_POS]) * DRV_POWER_ADC_VOLT_DIV ;
}

float drv_Power_get_mainVolt(void)
{
	return ((DRV_POWER_ADC_REF_VOLT / DRV_POWER_ADC_RESOLUTION) *
			(float)adcData[DRV_POWER_MAIN_POS]) * DRV_POWER_ADC_VOLT_DIV ;
}

void drv_Power_set_edlc_dcdc(FunctionalState state)
{
	if(state == ENABLE)
	{
		HAL_GPIO_WritePin(DCDC_ENABLE_GPIO_Port, DCDC_ENABLE_Pin, GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(DCDC_ENABLE_GPIO_Port, DCDC_ENABLE_Pin, GPIO_PIN_RESET);
	}
}
