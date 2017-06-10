#include "init.h"
#include "stm32f4xx_hal.h"
#include "main.h"

extern uint32_t HCLKFreq;
extern uint32_t APB1_timer_clock;
extern uint32_t Tim2_freq;
extern uint32_t Tim2_counter_clock;

void get_clock_status(void)
{
	/* Get clocks */
	HCLKFreq = HAL_RCC_GetHCLKFreq();	
	APB1_timer_clock = HCLKFreq;
	Tim2_freq = APB1_timer_clock;
	Tim2_counter_clock = (uint32_t) (Tim2_freq / (TIM2_PRESCALER+1));
}