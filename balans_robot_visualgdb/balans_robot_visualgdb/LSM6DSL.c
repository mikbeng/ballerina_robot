#include "i2c.h"
#include "LSM6DSL.h"
//#include "stm32f4xx_hal_i2c.h"

uint8_t ret_value;

void LSM6DSL_config(void)
{
	uint8_t CTRL_write1 = 0x80;
	//HAL_I2C_Mem_Write(&hi2c1, LSM6DSL_ADDRESS, ctrl_reg, 1, &CTRL_write1, 1, 100);	//Sets the DS3502 in mode 1
}

uint8_t LSM6DSL_read(void)
{
	/*
	uint8_t i2c_data[2];
	i2c_data[0] = 0x20;
	i2c_data[1] = wiper_wr;
	HAL_I2C_Mem_Read(&hi2c1, LSM6DSL_ADDRESS, WHO_AM_I_ADDRESS, 2, &ret_value, 2, 100);	//Sets the DS3502 in mode 1
	HAL_StatusTypeDef status = HAL_I2C_Master_Transmit(&hi2c1, LSM6DSL_ADDRESS, i2c_data, 2, 10);
	return ret_value;
	*/
}