/*
 * main_app.h
 *
 *  Created on: Sep 7, 2022
 *      Author: User
 */

#ifndef INC_MAIN_APP_H_
#define INC_MAIN_APP_H_

#include "main.h"

#define MAX_SINGLE_TASK		(8)

extern I2C_HandleTypeDef hi2c1;
extern RTC_HandleTypeDef hrtc;
extern SPI_HandleTypeDef hspi1;
extern TIM_HandleTypeDef htim6;
extern TIM_HandleTypeDef htim7;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart6;
extern DMA_HandleTypeDef hdma_usart3_rx;
extern DMA_HandleTypeDef hdma_usart3_tx;
extern TIM_HandleTypeDef *Timer_500ms;
extern uint8_t app_reflash_Flag;
extern uint8_t app_clear_Flag;
extern uint8_t oled_view_mode;
extern ADC_HandleTypeDef hadc1;

extern void main_app(void);
extern void app_oled_view_init(void);
extern void drv_set_app_timer(TIM_HandleTypeDef *timer);

extern void app_oled_view_temp(void);
extern void app_oled_view_lte(void);
extern void app_mqtt_send_tempData(void);
extern void app_mqtt_send_gpsData(void);
extern void app_lte_StartStop(void);
extern uint8_t app_add_onshot_Task (void *task);


#endif /* INC_MAIN_APP_H_ */
