/*
 * drv_bme280.h
 *
 *  Created on: 2022/08/15
 *      Author: User
 */

#ifndef INC_DRV_BME280_H_
#define INC_DRV_BME280_H_

#include "main.h"

extern struct bme280_dev dev1;

extern void drv_bme280_write_data(uint8_t *data,uint8_t len);
extern void drv_bme280_read_data(uint8_t addr,uint8_t *data,uint8_t len);
extern int8_t api_bme280_init(SPI_HandleTypeDef *spi_port,struct bme280_dev *dev);
extern int8_t stream_sensor_data_normal_mode(struct bme280_dev *dev,float *comp_data);


#endif /* INC_DRV_BME280_H_ */
