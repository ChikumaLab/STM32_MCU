/*
 * drv_bme280.c
 *
 *  Created on: 2022/08/15
 *      Author: User
 */


#include "drv_bme280.h"

#define DRV_BME280_REG_WRITE_MASK		(0x7F)
#define DRV_BME280_CTRL_HUM_REG_MASK	(0x07)
#define DRV_BME280_RESET_COMMAND        (0xB6)

SPI_HandleTypeDef *DRV_BME280_SPI_IF;
uint8_t drv_bme_tx_data[64];
uint8_t drv_bme_rx_data[64];

struct bme280_dev dev1;
int8_t  drv_bme280_read_function(uint8_t reg_addr,uint8_t *reg_data,uint32_t len, void *intf_ptr);
void user_delay_ms(uint32_t period, void *intf_ptr);
int8_t user_spi_write(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *intf_ptr);
uint8_t dev_addr = 0;

int8_t api_bme280_init(SPI_HandleTypeDef *spi_port,struct bme280_dev *dev)
{
	int8_t rslt = BME280_OK;
	DRV_BME280_SPI_IF = spi_port;

	dev[0].intf_ptr = &dev_addr;
	dev[0].intf = BME280_SPI_INTF;
	dev[0].read = drv_bme280_read_function;
	dev[0].write = (bme280_write_fptr_t)user_spi_write;
	dev[0].delay_us = user_delay_ms;

	rslt = bme280_init(dev);

	return rslt;
}
int8_t stream_sensor_data_normal_mode(struct bme280_dev *dev,float *comp_data)
{
	int8_t rslt;
	uint8_t settings_sel;
    struct bme280_data comp_data1;

	dev->settings.osr_h = BME280_OVERSAMPLING_1X;
	dev->settings.osr_p = BME280_OVERSAMPLING_16X;
	dev->settings.osr_t = BME280_OVERSAMPLING_2X;
	dev->settings.filter = BME280_FILTER_COEFF_16;
	dev->settings.standby_time = BME280_STANDBY_TIME_62_5_MS;

	settings_sel = BME280_OSR_PRESS_SEL;
	settings_sel |= BME280_OSR_TEMP_SEL;
	settings_sel |= BME280_OSR_HUM_SEL;
	settings_sel |= BME280_STANDBY_SEL;
	settings_sel |= BME280_FILTER_SEL;
	rslt = bme280_set_sensor_settings(settings_sel, dev);
	rslt = bme280_set_sensor_mode(BME280_NORMAL_MODE, dev);
	dev->delay_us(70000, dev->intf_ptr);
	rslt = bme280_get_sensor_data(BME280_ALL, &comp_data1, dev);

	comp_data[0] = (float)comp_data1.temperature;
	comp_data[1] = (float)(comp_data1.pressure / 100.0);
	comp_data[2] = (float)comp_data1.humidity;


	return rslt;
}

int8_t  drv_bme280_read_function(uint8_t reg_addr,uint8_t *reg_data,uint32_t len, void *intf_ptr)
{
	int8_t rslt = 0; /* Return 0 for Success, non-zero for failure */
	drv_bme280_read_data(reg_addr,reg_data,(uint8_t)len);
	return rslt ;
}

void user_delay_ms(uint32_t period, void *intf_ptr)
{
    /*
     * Return control or wait,
     * for a period amount of milliseconds
     */
	uint32_t wait = (period/1000) + 1;

	HAL_Delay(wait);
}


int8_t user_spi_write(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *intf_ptr)
{
    int8_t rslt = 0; /* Return 0 for Success, non-zero for failure */
	uint8_t i;
	drv_bme_tx_data[0] = reg_addr;
	for(i = 0;i <len;i++)
	{
		drv_bme_tx_data[i + 1] = reg_data[i];
	}
	HAL_GPIO_WritePin(SPI1_CS_GPIO_Port,SPI1_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(DRV_BME280_SPI_IF,drv_bme_tx_data,len + 1,1000);
	HAL_GPIO_WritePin(SPI1_CS_GPIO_Port,SPI1_CS_Pin, GPIO_PIN_SET);

    return rslt;

}
void drv_bme280_write_data(uint8_t *data,uint8_t len)
{
	uint8_t i;
	if(len > 32)
	{
		return;
	}
	else
	{
		for(i = 0;i <len;i++)
		{
			drv_bme_tx_data[i] = data[i];
		}
		HAL_GPIO_WritePin(SPI1_CS_GPIO_Port,SPI1_CS_Pin, GPIO_PIN_RESET);
		HAL_SPI_Transmit(DRV_BME280_SPI_IF,drv_bme_tx_data,len + 1,1000);
		HAL_GPIO_WritePin(SPI1_CS_GPIO_Port,SPI1_CS_Pin, GPIO_PIN_SET);
	}
}

void drv_bme280_read_data(uint8_t addr,uint8_t *data,uint8_t len)
{
	if(len > 64)
	{
		return;
	}
	else
	{
		drv_bme_tx_data[0] = addr;
		HAL_GPIO_WritePin(SPI1_CS_GPIO_Port,SPI1_CS_Pin, GPIO_PIN_RESET);
		HAL_SPI_Transmit(DRV_BME280_SPI_IF,drv_bme_tx_data,1,1000);
		HAL_SPI_Receive(DRV_BME280_SPI_IF,data,len,1000);
		HAL_GPIO_WritePin(SPI1_CS_GPIO_Port,SPI1_CS_Pin, GPIO_PIN_SET);
	}
}

