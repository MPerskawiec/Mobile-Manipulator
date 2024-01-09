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
#define SERVO_6_PWM_Pin GPIO_PIN_13
#define SERVO_6_PWM_GPIO_Port GPIOC
#define LED_Pin GPIO_PIN_5
#define LED_GPIO_Port GPIOA
#define HC_3_ECHO_Pin GPIO_PIN_4
#define HC_3_ECHO_GPIO_Port GPIOC
#define HC_3_ECHO_EXTI_IRQn EXTI4_IRQn
#define PK_2A_Pin GPIO_PIN_5
#define PK_2A_GPIO_Port GPIOC
#define HC_1_ECHO_Pin GPIO_PIN_1
#define HC_1_ECHO_GPIO_Port GPIOB
#define HC_1_ECHO_EXTI_IRQn EXTI1_IRQn
#define HC_1_TRIG_Pin GPIO_PIN_2
#define HC_1_TRIG_GPIO_Port GPIOB
#define LK_2A_Pin GPIO_PIN_12
#define LK_2A_GPIO_Port GPIOB
#define HC_3_TRIG_Pin GPIO_PIN_13
#define HC_3_TRIG_GPIO_Port GPIOB
#define HC_2_ECHO_Pin GPIO_PIN_14
#define HC_2_ECHO_GPIO_Port GPIOB
#define HC_2_ECHO_EXTI_IRQn EXTI15_10_IRQn
#define HC_2_TRIG_Pin GPIO_PIN_15
#define HC_2_TRIG_GPIO_Port GPIOB
#define PK_1A_Pin GPIO_PIN_6
#define PK_1A_GPIO_Port GPIOC
#define PWM_PK_Pin GPIO_PIN_8
#define PWM_PK_GPIO_Port GPIOC
#define HC_4_ECHO_Pin GPIO_PIN_10
#define HC_4_ECHO_GPIO_Port GPIOA
#define HC_4_ECHO_EXTI_IRQn EXTI15_10_IRQn
#define LK_1A_Pin GPIO_PIN_11
#define LK_1A_GPIO_Port GPIOA
#define PWM_LK_Pin GPIO_PIN_12
#define PWM_LK_GPIO_Port GPIOA
#define SERVO_4_PWM_Pin GPIO_PIN_15
#define SERVO_4_PWM_GPIO_Port GPIOA
#define SERVO_3_PWM_Pin GPIO_PIN_10
#define SERVO_3_PWM_GPIO_Port GPIOC
#define SERVO_1_PWM_Pin GPIO_PIN_11
#define SERVO_1_PWM_GPIO_Port GPIOC
#define SERVO_2_PWM_Pin GPIO_PIN_12
#define SERVO_2_PWM_GPIO_Port GPIOC
#define SERVO_7_PWM_Pin GPIO_PIN_2
#define SERVO_7_PWM_GPIO_Port GPIOD
#define HC_4_TRIG_Pin GPIO_PIN_3
#define HC_4_TRIG_GPIO_Port GPIOB
#define SERVO_5_PWM_Pin GPIO_PIN_7
#define SERVO_5_PWM_GPIO_Port GPIOB
#define PWM_LK2_Pin GPIO_PIN_9
#define PWM_LK2_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
