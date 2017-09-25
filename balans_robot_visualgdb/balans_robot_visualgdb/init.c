#include "init.h"
#include "stm32f4xx_hal.h"
#include "main.h"
#include "tim.h"
#include "stepper.h"
#include "i2c.h"

extern uint32_t HCLKFreq;
extern uint32_t APB1_timer_clock;
extern uint32_t Tim2_freq;
extern uint32_t Tim2_counter_clock;

void HAL_GPIO_WRITE_ODR(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void HAL_I2C_ClearBusyFlagErrata_2_14_7(I2C_HandleTypeDef *hi2c);


void init_system(void)
{
	//HAL_I2C_ClearBusyFlagErrata_2_14_7(&hi2c1);
	
	//Inits
	//HTS221_init();
	LSM6DSL_init();
	stepper_setstep(STEP_SIZE_USE,STEPPER_1);
	stepper_setstep(STEP_SIZE_USE, STEPPER_2);
	get_clock_status();
	SET_BIT(TIM2->CR1, TIM_CR1_ARPE);	//Enables ARPE BIT in CR1 reg for TIM2_ARR auto reload mode
	SET_BIT(TIM2->CR1, TIM_CR1_URS);	//Only counter overflow/underflow generates an update interrupt or DMA request
}

void get_clock_status(void)
{
	/* Get clocks */
	HCLKFreq = HAL_RCC_GetHCLKFreq();	
	APB1_timer_clock = HCLKFreq;
	Tim2_freq = APB1_timer_clock;
	Tim2_counter_clock = (uint32_t) (Tim2_freq / (TIM2_PRESCALER+1));
}

void HAL_I2C_ClearBusyFlagErrata_2_14_7(I2C_HandleTypeDef *hi2c) {

	static uint8_t resetTried = 0;
	if (resetTried == 1) {
		return ;
	}
	uint32_t SDA_PIN = GPIO_PIN_9;
	uint32_t SCL_PIN = GPIO_PIN_8;
	GPIO_InitTypeDef GPIO_InitStruct;

	// 1
	__HAL_I2C_DISABLE(hi2c);

	// 2
	GPIO_InitStruct.Pin = SDA_PIN | SCL_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	HAL_GPIO_WRITE_ODR(GPIOB, SDA_PIN);
	HAL_GPIO_WRITE_ODR(GPIOB, SCL_PIN);

	// 3
	GPIO_PinState pinState;
	if (HAL_GPIO_ReadPin(GPIOB, SDA_PIN) == GPIO_PIN_RESET) {
		for (;;) {}
	}
	if (HAL_GPIO_ReadPin(GPIOB, SCL_PIN) == GPIO_PIN_RESET) {
		for (;;) {}
	}

	// 4
	GPIO_InitStruct.Pin = SDA_PIN;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	HAL_GPIO_TogglePin(GPIOB, SDA_PIN);

	// 5
	if(HAL_GPIO_ReadPin(GPIOB, SDA_PIN) == GPIO_PIN_SET) {
		for (;;) {}
	}

	// 6
	GPIO_InitStruct.Pin = SCL_PIN;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	HAL_GPIO_TogglePin(GPIOB, SCL_PIN);

	// 7
	if(HAL_GPIO_ReadPin(GPIOB, SCL_PIN) == GPIO_PIN_SET) {
		for (;;) {}
	}

	// 8
	GPIO_InitStruct.Pin = SDA_PIN;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	HAL_GPIO_WRITE_ODR(GPIOB, SDA_PIN);

	// 9
	if(HAL_GPIO_ReadPin(GPIOB, SDA_PIN) == GPIO_PIN_RESET) {
		for (;;) {}
	}

	// 10
	GPIO_InitStruct.Pin = SCL_PIN;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	HAL_GPIO_WRITE_ODR(GPIOB, SCL_PIN);

	// 11
	if(HAL_GPIO_ReadPin(GPIOB, SCL_PIN) == GPIO_PIN_RESET) {
		for (;;) {}
	}

	// 12
	GPIO_InitStruct.Pin = SDA_PIN | SCL_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
	GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	// 13
	hi2c->Instance->CR1 |= I2C_CR1_SWRST;

	// 14
	hi2c->Instance->CR1 ^= I2C_CR1_SWRST;

	// 15
	__HAL_I2C_ENABLE(hi2c);

	resetTried = 1;
}

void HAL_GPIO_WRITE_ODR(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	/* Check the parameters */
	assert_param(IS_GPIO_PIN(GPIO_Pin));

	GPIOx->ODR |= GPIO_Pin;
}