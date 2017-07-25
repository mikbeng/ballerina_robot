#include "stdio.h"
#include "stepper.h"
#include "gpio.h"
#include "math.h"
#include "tim.h"

extern uint32_t Tim2_counter_clock;

void stepper_setstep(uint32_t step_size_used, uint32_t stepper_val)
{

	if (stepper_val == 1)
	{
		//For stepper 1
		//PC1 -> MS1
		//PC2 -> MS2
		//PC3 -> MS3
		switch (step_size_used)
		{
		case 1  :
			HAL_GPIO_WritePin(stepper1_MS1_GPIO_Port, stepper1_MS1_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(stepper1_MS2_GPIO_Port, stepper1_MS2_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(stepper1_MS3_GPIO_Port, stepper1_MS3_Pin, GPIO_PIN_RESET);
			break; /* optional */

		case 2  :
			HAL_GPIO_WritePin(stepper1_MS1_GPIO_Port, stepper1_MS1_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(stepper1_MS2_GPIO_Port, stepper1_MS2_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(stepper1_MS3_GPIO_Port, stepper1_MS3_Pin, GPIO_PIN_RESET);
			break; /* optional */

		case 4  :
			HAL_GPIO_WritePin(stepper1_MS1_GPIO_Port, stepper1_MS1_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(stepper1_MS2_GPIO_Port, stepper1_MS2_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(stepper1_MS3_GPIO_Port, stepper1_MS3_Pin, GPIO_PIN_RESET);
			break; /* optional */

		case 8  :
			HAL_GPIO_WritePin(stepper1_MS1_GPIO_Port, stepper1_MS1_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(stepper1_MS2_GPIO_Port, stepper1_MS2_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(stepper1_MS3_GPIO_Port, stepper1_MS3_Pin, GPIO_PIN_RESET);
			break; /* optional */

		case 16  :
			HAL_GPIO_WritePin(stepper1_MS1_GPIO_Port, stepper1_MS1_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(stepper1_MS2_GPIO_Port, stepper1_MS2_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(stepper1_MS3_GPIO_Port, stepper1_MS3_Pin, GPIO_PIN_SET);
			break; /* optional */
		}
	}
	else if (stepper_val == 2)
	{
		//For stepper 2
		//PC10 -> MS1
		//PC12 -> MS2
		//PD2 -> MS3
		switch (step_size_used)
		{
		case 1  :
			HAL_GPIO_WritePin(stepper2_MS1_GPIO_Port, stepper2_MS1_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(stepper2_MS2_GPIO_Port, stepper2_MS2_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(stepper2_MS3_GPIO_Port, stepper2_MS3_Pin, GPIO_PIN_RESET);
			break; /* optional */

		case 2  :
			HAL_GPIO_WritePin(stepper2_MS1_GPIO_Port, stepper2_MS1_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(stepper2_MS2_GPIO_Port, stepper2_MS2_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(stepper2_MS3_GPIO_Port, stepper2_MS3_Pin, GPIO_PIN_RESET);
			break; /* optional */

		case 4  :
			HAL_GPIO_WritePin(stepper2_MS1_GPIO_Port, stepper2_MS1_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(stepper2_MS2_GPIO_Port, stepper2_MS2_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(stepper2_MS3_GPIO_Port, stepper2_MS3_Pin, GPIO_PIN_RESET);
			break; /* optional */

		case 8  :
			HAL_GPIO_WritePin(stepper2_MS1_GPIO_Port, stepper2_MS1_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(stepper2_MS2_GPIO_Port, stepper2_MS2_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(stepper2_MS3_GPIO_Port, stepper2_MS3_Pin, GPIO_PIN_RESET);
			break; /* optional */

		case 16  :
			HAL_GPIO_WritePin(stepper2_MS1_GPIO_Port, stepper2_MS1_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(stepper2_MS2_GPIO_Port, stepper2_MS2_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(stepper2_MS3_GPIO_Port, stepper2_MS3_Pin, GPIO_PIN_SET);
			break; /* optional */
		}
	}
}

void send_ang_velocity(float w_rad)	//in radians
{
	uint32_t PWM_freq;
	uint32_t PWM_ARR;
	float w_deg = 0;
	float w_rad_abs = 0;
	
	//CHeck if PWM is off
	if ((READ_BIT(TIM2->CR1, TIM_CR1_CEN)) == 0)
	{
		HAL_TIM_OC_Start(&htim2, TIM_CHANNEL_1);	//start PWM CH1
		HAL_TIM_OC_Start(&htim2, TIM_CHANNEL_2);	//start PWM CH2
	}
	
	w_rad_abs = fabs(w_rad);
	w_deg = w_rad_abs*(180 / M_PI);	 //degrees per sec
	PWM_freq = (uint32_t)(w_deg / (STEP_SIZE / STEP_SIZE_USE));
	
	if (PWM_freq == 0)
	{
		PWM_ARR = 0xFFFFFFFF;
	}
	else
	{
		PWM_ARR = (uint32_t)(Tim2_counter_clock / PWM_freq);
	}
			
	//Update preload registers TIM2
	
	SET_BIT(TIM2->CR1, TIM_CR1_UDIS);	//Set UDIS bit to disable UEV while writing to the preload registers
	
	TIM2->ARR = PWM_ARR;
	TIM2->CCR1 = (uint32_t)(PWM_ARR / 2);		//Always 50% duty cycle	
	TIM2->CCR2 = (uint32_t)(PWM_ARR / 2);		//Always 50% duty cycle	
			
	CLEAR_BIT(TIM2->CR1, TIM_CR1_UDIS);	//Clear UDIS bit to enable UEV again.
	
	SET_BIT(TIM2->EGR, TIM_EGR_UG);		//Re-initialize the counter and generates an update of the registers.


	if (w_rad > 0)
	{
		HAL_GPIO_WritePin(GPIOC, stepper1_dir_Pin, STEPPER1_POS_DIR);
		HAL_GPIO_WritePin(GPIOC, stepper2_dir_Pin, STEPPER2_POS_DIR);
	}
	else if (w_rad <= 0)
	{
		HAL_GPIO_WritePin(GPIOC, stepper1_dir_Pin, STEPPER1_NEG_DIR);
		HAL_GPIO_WritePin(GPIOC, stepper2_dir_Pin, STEPPER2_NEG_DIR);
	}
	

}