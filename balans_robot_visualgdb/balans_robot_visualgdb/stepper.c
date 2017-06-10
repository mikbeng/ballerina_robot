#include "stdio.h"
#include "stepper.h"
#include "gpio.h"
#include "math.h"
#include "tim.h"

extern uint32_t Tim2_counter_clock;

void stepper1_init()
{
	//Step resolution
	GPIOB->ODR &= ~(STEP_SIZE_1_16); 

	HAL_TIM_OC_Start(&htim2, TIM_CHANNEL_1);	//start PWM
}

void send_velocity(float v)
{
	uint32_t PWM_freq;
	uint32_t PWM_ARR;
	float w_rad = 0;
	float w_deg = 0;

	if (v == 0)
	{
		//Stop PWM
		HAL_TIM_OC_Stop(&htim2, TIM_CHANNEL_1);
	}
	else if (v > 0)
	{
		w_rad = v / (WHEEL_D / 2);	 //rad per sec
		w_deg = w_rad*(180 / M_PI);	 //degrees per sec
		PWM_freq = (uint32_t)(w_deg / (STEP_SIZE*STEP_SIZE_1_16));
		PWM_ARR = (uint32_t)(Tim2_counter_clock / PWM_freq);
		TIM2->ARR = PWM_ARR;
		TIM2->CCR1 = (uint32_t)(PWM_ARR / 2);		//Always 50% duty cycle
		//CHeck if PWM is on
		HAL_TIM_StateTypeDef HAL_TIM_OC_GetState(ASDF,&htim2);
		HAL_TIM_OC_Start(&htim2, TIM_CHANNEL_1);	//start PWM
	}
	else
	{
		
	}

}