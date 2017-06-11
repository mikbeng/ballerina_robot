#include "init.h"
#include "stm32f4xx_hal.h"
#include "main.h"
#include "tim.h"
#include "stepper.h"

extern uint32_t HCLKFreq;
extern uint32_t APB1_timer_clock;
extern uint32_t Tim2_freq;
extern uint32_t Tim2_counter_clock;

void init_system(void)
{
	//Inits
	HTS221_init();
	LSM6DSL_init();
	stepper1_setstep(STEP_SIZE_1_16);
	get_clock_status();
	SET_BIT(TIM2->CR1, TIM_CR1_ARPE);

	//TIM2->CR1 &= ~(TIM_CR1_ARPE_Msk);		//Enables ARPE BIT in CR1 reg for TIM2_ARR auto reload mode
}

void get_clock_status(void)
{
	/* Get clocks */
	HCLKFreq = HAL_RCC_GetHCLKFreq();	
	APB1_timer_clock = HCLKFreq;
	Tim2_freq = APB1_timer_clock;
	Tim2_counter_clock = (uint32_t) (Tim2_freq / (TIM2_PRESCALER+1));
}