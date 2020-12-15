/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#include "stm32g4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
#define thruster1_pin_Pin GPIO_PIN_0
#define thruster1_pin_GPIO_Port GPIOA
#define thruster2_pin_Pin GPIO_PIN_1
#define thruster2_pin_GPIO_Port GPIOA
#define thruster3_pin_Pin GPIO_PIN_2
#define thruster3_pin_GPIO_Port GPIOA
#define thruster4_pin_Pin GPIO_PIN_3
#define thruster4_pin_GPIO_Port GPIOA
#define thruster5_pin_Pin GPIO_PIN_6
#define thruster5_pin_GPIO_Port GPIOA
#define channel4_pin_Pin GPIO_PIN_7
#define channel4_pin_GPIO_Port GPIOA
#define thruster6_pin_Pin GPIO_PIN_0
#define thruster6_pin_GPIO_Port GPIOB
#define red_light_pin_Pin GPIO_PIN_9
#define red_light_pin_GPIO_Port GPIOA
#define green_light_pin_Pin GPIO_PIN_10
#define green_light_pin_GPIO_Port GPIOA
#define blue_light_pin_Pin GPIO_PIN_11
#define blue_light_pin_GPIO_Port GPIOA
#define channel2_pin_Pin GPIO_PIN_12
#define channel2_pin_GPIO_Port GPIOA
#define bilge_pump_pin_Pin GPIO_PIN_5
#define bilge_pump_pin_GPIO_Port GPIOB
#define channel5_pin_Pin GPIO_PIN_8
#define channel5_pin_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
