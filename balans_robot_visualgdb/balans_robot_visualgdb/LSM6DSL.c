#include "i2c.h"
#include "LSM6DSL.h"


void LSM6DSL_init(void)
{
	uint8_t signature;

	uint8_t ctrl_reg1_XL_conf = 0b01010000;			//208Hz Normal mode. FS=+-2g
	uint8_t ctrl_reg2_G_conf = 0b01010000;			//208Hz Normal mode. FS=245dps
	uint8_t ctrl_reg3_C_conf = 0b01000100;			//BDU=1. IF_INC=1 (default)

	HAL_I2C_Mem_Read(&hi2c1, LSM6DSL_ADDRESS, LSM6DSL_WHO_AM_I_REG, 1, &signature, 1, 100);	//Get device signature
	
	if (signature == LSM6DSL_ACC_GYRO_WHO_AM_I)
	{
		//Config device
		HAL_I2C_Mem_Write(&hi2c1, LSM6DSL_ADDRESS, LSM6DSL_CTRL1_XL, 1, &ctrl_reg1_XL_conf, 1, 10);	
		HAL_I2C_Mem_Write(&hi2c1, LSM6DSL_ADDRESS, LSM6DSL_CTRL2_G, 1, &ctrl_reg2_G_conf, 1, 10);
		HAL_I2C_Mem_Write(&hi2c1, LSM6DSL_ADDRESS, LSM6DSL_CTRL3_C, 1, &ctrl_reg3_C_conf, 1, 10);
		
		//Calibration?	
	}
	else
	{
		//Init error
	}
}
