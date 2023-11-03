/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include "arm_math.h"

#include "main_app.h"

#include "drv_lteModule.h"
#include "drv_cmd_If.h"
#include "app_sys_cmd.h"
#include "app_lte_cmd.h"
#include "drv_ae_aqm1602a.h"
#include "drv_oled_ssd1306.h"
#include "drv_bme280.h"
#include "bme280.h"
#include "drv_gps.h"
#include "drv_xbee_wifi.h"

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define XBEE_RESET_Pin GPIO_PIN_0
#define XBEE_RESET_GPIO_Port GPIOC
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA
#define SPI1_CS_Pin GPIO_PIN_10
#define SPI1_CS_GPIO_Port GPIOB
#define P_SW_Pin GPIO_PIN_8
#define P_SW_GPIO_Port GPIOC
#define P_SW_EXTI_IRQn EXTI9_5_IRQn
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define LTE_V_INT_Pin GPIO_PIN_15
#define LTE_V_INT_GPIO_Port GPIOA
#define LTE_UART_TXD_Pin GPIO_PIN_10
#define LTE_UART_TXD_GPIO_Port GPIOC
#define LTE_UART_RXD_Pin GPIO_PIN_11
#define LTE_UART_RXD_GPIO_Port GPIOC
#define LTE_BUF_EN_Pin GPIO_PIN_2
#define LTE_BUF_EN_GPIO_Port GPIOD
#define LTE_PWR_ON_Pin GPIO_PIN_7
#define LTE_PWR_ON_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
