/*
 * drv_oled_ssd1306.h
 *
 *  Created on: 2022/08/01
 *      Author: User
 */

#ifndef INC_DRV_OLED_SSD1306_H_
#define INC_DRV_OLED_SSD1306_H_

#include "main.h"

#define AE_SSD1306_I2C_ADDR	(0x78)
extern void drv_oled_ssd1306_init(I2C_HandleTypeDef *i2c);
extern void drv_oled_ssd1306_clear(void);
extern void drv_oled_ssd1306_font_posi(uint8_t page, uint8_t column);
extern void drv_oled_ssd1306_set_char(uint8_t c);        // 6x8dotフォントの1文字表示関数
extern void drv_oled_ssd1306_set_string(uint8_t x,uint8_t y,uint8_t *data,uint8_t len);
extern void drv_oled_ssd1306_set_h_line(uint8_t x,uint8_t y,uint8_t len);
extern void drv_oled_ssd1306_set_ant_param(uint8_t data);
extern void drv_oled_ssd1306_set_batt_param(uint8_t data);
extern void drv_oled_ssd1306_set_temp_param(float data);
extern void drv_oled_ssd1306_set_humdity_param(float data);
extern void drv_oled_ssd1306_set_presser_param(float data);
extern void drv_oled_ssd1306_set_time(void);
extern void drv_oled_ssd1306_set_lte_qual(uint8_t rssi,uint8_t rsrp,uint8_t rsrq, uint8_t sinr);
extern void drv_oled_ssd1306_set_lte_cellData(uint8_t tac,uint8_t cell);
extern void drv_oled_ssd1306_set_lte_band(uint8_t band,uint8_t bandwide,uint8_t ulink);
extern void drv_oled_ssd1306_set_solar_param(float data);
extern void drv_oled_ssd1306_set_main_param(float data);
extern void drv_oled_ssd1306_set_edlc_param(float data);

//extern void h_line(uint8_t page, uint8_t start_column, uint8_t length,uint8_t x_position) ;
extern const char font6[91][6];

#endif /* INC_DRV_OLED_SSD1306_H_ */
