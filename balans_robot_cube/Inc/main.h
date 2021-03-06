/**
  ******************************************************************************
  * File Name          : main.h
  * Description        : This file contains the common defines of the application
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2017 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H
  /* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */
#include <stdint.h>
/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/
#define TIM2_PRESCALER 20

#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define stepper1_dir_Pin GPIO_PIN_0
#define stepper1_dir_GPIO_Port GPIOC
#define stepper1_MS1_Pin GPIO_PIN_1
#define stepper1_MS1_GPIO_Port GPIOC
#define stepper1_MS2_Pin GPIO_PIN_2
#define stepper1_MS2_GPIO_Port GPIOC
#define stepper1_MS3_Pin GPIO_PIN_3
#define stepper1_MS3_GPIO_Port GPIOC
#define stepper1_step_Pin GPIO_PIN_0
#define stepper1_step_GPIO_Port GPIOA
#define stepper2_step_Pin GPIO_PIN_1
#define stepper2_step_GPIO_Port GPIOA
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define MLX1_CS_Pin GPIO_PIN_4
#define MLX1_CS_GPIO_Port GPIOA
#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA
#define MLX1_MOSI_MISO_Pin GPIO_PIN_4
#define MLX1_MOSI_MISO_GPIO_Port GPIOC
#define stepper1_EN_Pin GPIO_PIN_12
#define stepper1_EN_GPIO_Port GPIOB
#define stepper2_EN_Pin GPIO_PIN_13
#define stepper2_EN_GPIO_Port GPIOB
#define stepper1_SLEEP_Pin GPIO_PIN_6
#define stepper1_SLEEP_GPIO_Port GPIOC
#define stepper2_SLEEP_Pin GPIO_PIN_7
#define stepper2_SLEEP_GPIO_Port GPIOC
#define stepper1_RST_Pin GPIO_PIN_8
#define stepper1_RST_GPIO_Port GPIOC
#define stepper2_RST_Pin GPIO_PIN_9
#define stepper2_RST_GPIO_Port GPIOC
#define MLX1_PULL_Pin GPIO_PIN_10
#define MLX1_PULL_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define stepper2_MS1_Pin GPIO_PIN_15
#define stepper2_MS1_GPIO_Port GPIOA
#define MLX1_SCLK_Pin GPIO_PIN_10
#define MLX1_SCLK_GPIO_Port GPIOC
#define stepper2_dir_Pin GPIO_PIN_11
#define stepper2_dir_GPIO_Port GPIOC
#define stepper2_MS2_Pin GPIO_PIN_12
#define stepper2_MS2_GPIO_Port GPIOC
#define stepper2_MS3_Pin GPIO_PIN_2
#define stepper2_MS3_GPIO_Port GPIOD
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
#define STEPPER_1 (1)
#define STEPPER_2 (2)
#define R_WHEEL (0.06/2)
#define TS 0.1
//#define I_WHEEL 0.00001156 
#define I_WHEEL 0.001	//Check this parameter! 

//Function prototypes
void init_system(void);
void LSM6DSL_init(void);
//Global variables
extern float state_vector[4];

extern uint64_t systemTime;
extern float length, speed;
extern int32_t quadrantCount, quadrantCount2;
extern float lengthOffset;
extern float maxLength;
extern uint8_t batteryStatus;
extern uint8_t currentQuadrant, currentQuadrant2;
extern float GVNPressure;

/* USER CODE END Private defines */

void _Error_Handler(char *, int);

#define Error_Handler() _Error_Handler(__FILE__, __LINE__)

/**
  * @}
  */ 

/**
  * @}
*/ 

#endif /* __MAIN_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
