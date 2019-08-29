/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
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
#include "stm32f1xx_hal.h"

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

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define In1_Pin GPIO_PIN_0
#define In1_GPIO_Port GPIOE
#define In2_Pin GPIO_PIN_1
#define In2_GPIO_Port GPIOE
#define In3_Pin GPIO_PIN_2
#define In3_GPIO_Port GPIOE
#define In4_Pin GPIO_PIN_3
#define In4_GPIO_Port GPIOE
#define In5_Pin GPIO_PIN_4
#define In5_GPIO_Port GPIOE
#define In6_Pin GPIO_PIN_5
#define In6_GPIO_Port GPIOE
#define Led_Pin GPIO_PIN_6
#define Led_GPIO_Port GPIOE
#define Ctrl1_Pin GPIO_PIN_0
#define Ctrl1_GPIO_Port GPIOC
#define Ctrl2_Pin GPIO_PIN_1
#define Ctrl2_GPIO_Port GPIOC
#define Ctrl3_Pin GPIO_PIN_2
#define Ctrl3_GPIO_Port GPIOC
#define Ctrl4_Pin GPIO_PIN_3
#define Ctrl4_GPIO_Port GPIOC
#define Ctrl5_Pin GPIO_PIN_4
#define Ctrl5_GPIO_Port GPIOC
#define Ctrl6_Pin GPIO_PIN_5
#define Ctrl6_GPIO_Port GPIOC
#define Ctrl7_Pin GPIO_PIN_6
#define Ctrl7_GPIO_Port GPIOC
#define Ctrl8_Pin GPIO_PIN_7
#define Ctrl8_GPIO_Port GPIOC

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
