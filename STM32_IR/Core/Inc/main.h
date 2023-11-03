/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "stm32f3xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "drv_ir_cnt.h"

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

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED1_Pin GPIO_PIN_0
#define LED1_GPIO_Port GPIOA
#define LED2_Pin GPIO_PIN_1
#define LED2_GPIO_Port GPIOA
#define EXT_PSW_Pin GPIO_PIN_0
#define EXT_PSW_GPIO_Port GPIOB
#define EXT_PSW_EXTI_IRQn EXTI0_IRQn
#define IR_SW_Pin GPIO_PIN_1
#define IR_SW_GPIO_Port GPIOB
#define SPI_SW_Pin GPIO_PIN_8
#define SPI_SW_GPIO_Port GPIOA
#define DIPSW0_Pin GPIO_PIN_11
#define DIPSW0_GPIO_Port GPIOA
#define DIPSW1_Pin GPIO_PIN_12
#define DIPSW1_GPIO_Port GPIOA
#define EXT_IR_Pin GPIO_PIN_4
#define EXT_IR_GPIO_Port GPIOB
#define EXT_IR_EXTI_IRQn EXTI4_IRQn
#define LCD_SW_Pin GPIO_PIN_6
#define LCD_SW_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
