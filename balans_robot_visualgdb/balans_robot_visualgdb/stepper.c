#include "stdio.h"
#include "stepper.h"
#include "gpio.h"
#include "math.h"
#include "tim.h"

extern uint32_t Tim2_counter_clock;

void stepper1_setstep(uint32_t step_size_used)
{
	switch (step_size_used)
	{
	case 1  :
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_RESET);
		break; /* optional */

	case 2  :
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_RESET);
		break; /* optional */

	case 4  :
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_RESET);
		break; /* optional */

	case 8  :
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_RESET);
		break; /* optional */

	case 16  :
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET);
		break; /* optional */

	case 32  :
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET);
		break; /* optional */
	}

}

void send_velocity(float v)
{
	uint32_t PWM_freq;
	uint32_t PWM_ARR;
	float w_rad = 0;
	float w_deg = 0;
	float v_abs = 0;

	//v = abs(v);

	if (v <= 0.1)
	{
		//Stop PWM
		HAL_TIM_OC_Stop(&htim2, TIM_CHANNEL_1);
	}

	//Check sign
		//If change in sign from prevoius. Change dir.
	//
	

	/*
	else if ((v_abs > 0.1) && (v_abs <= 20))
	{
		stepper1_setstep(STEP_SIZE_1_32);
	}
	else if ((v_abs > 20) && (v_abs <= V_ABS_LIM))
	{
		stepper1_setstep(STEP_SIZE_1_16);
	}*/

	
	else if (v > 0.1)
	{
		w_rad = v / (WHEEL_D / 2);	 //centi-rad per sec
		w_deg = w_rad*(180 / M_PI);	 //centi-degrees per sec
		PWM_freq = (uint32_t)(w_deg / (STEP_SIZE / STEP_SIZE_USE));
		PWM_ARR = (uint32_t)(Tim2_counter_clock / PWM_freq);
		TIM2->ARR = PWM_ARR;
		TIM2->CCR1 = (uint32_t)(PWM_ARR / 2);		//Always 50% duty cycle
		
		//CHeck if PWM is off
		if ((READ_BIT(TIM2->CR1, TIM_CR1_CEN)) == 0)
		{
			HAL_TIM_OC_Start(&htim2, TIM_CHANNEL_1);	//start PWM
		}
		
	}
	else
	{
		//Negative velocity??
	}

}