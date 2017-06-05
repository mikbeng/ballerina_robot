#include "i2c.h"
#include "LSM6DSL.h"


//Global variables for LSM6DSL.c
float acc_sensitivity = 0.0;
float gyro_sensitivity = 0.0;

void LSM6DSL_Get_config(void);

void LSM6DSL_init(void)
{
	uint8_t signature;

	uint8_t ctrl_reg1_XL_conf = 0b01010000;			//208Hz Normal mode. FS=+-2g
	uint8_t ctrl_reg2_G_conf = 0b01010000;			//208Hz Normal mode. FS=245dps	
	uint8_t ctrl_reg3_C_conf = 0b01000100;			//BDU=1. IF_INC=1 (default)

	HAL_I2C_Mem_Read(&hi2c1, LSM6DSL_ADDRESS, LSM6DSL_WHO_AM_I_REG, 1, &signature, 1, 100);	//Get device signature
	
	if (signature == LSM6DSL_ACC_GYRO_WHO_AM_I)
	{
		//Config Accelerometer and gyro
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

void LSM6DSL_Get_config(void)
{

	uint8_t ctrl_reg1_XL_read = 0;
	uint8_t fs_acc = 0;
	uint8_t ctrl_reg2_G_read = 0;
	uint8_t fs_gyro = 0;
	

	//Get FS acc configuration, see datasheet
	HAL_I2C_Mem_Read(&hi2c1, LSM6DSL_ADDRESS, LSM6DSL_CTRL1_XL, 1, &ctrl_reg1_XL_read, 1, 10);	
	fs_acc = (ctrl_reg1_XL_read & LSM6DSL_ACC_FS_XL_MASK);
	switch (fs_acc)
	{
	case LSM6DSL_ACC_FS_XL_2g:
		acc_sensitivity = 0.061;
		break;

	case LSM6DSL_ACC_FS_XL_4g:
		acc_sensitivity = 0.0122;
		break;

	case LSM6DSL_ACC_FS_XL_8g:
		acc_sensitivity = 0.0244;
		break;

	case LSM6DSL_ACC_FS_XL_16g:
		acc_sensitivity = 0.0488;
		break;
	}


	//Get FS gyro configuration, see datasheet
	HAL_I2C_Mem_Read(&hi2c1, LSM6DSL_ADDRESS, LSM6DSL_CTRL2_G, 1, &ctrl_reg2_G_read, 1, 10);	
	fs_gyro = (ctrl_reg2_G_read & LSM6DSL_GYRO_FS_MASK);
	switch (fs_gyro)
	{
	case LSM6DSL_GYRO_FS_125DPS:
		gyro_sensitivity = 4.375 / 1000;
		break;

	case LSM6DSL_GYRO_FS_245DPS:
		gyro_sensitivity = 8.75 / 1000;
		break;

	case LSM6DSL_GYRO_FS_500DSP:
		gyro_sensitivity = 17.50 / 1000;
		break;

	case LSM6DSL_GYRO_FS_1000DPS:
		gyro_sensitivity = 35.0 / 1000;
		break;

	case LSM6DSL_GYRO_FS_2000DPS:
		gyro_sensitivity = 70.0 / 1000;
		break;
	}


}

void LSM6DSL_Get_Acc(float *array_data_acc)
{
	uint8_t LSM6DSL_STATUS = 0;	

	uint8_t OutX_L_XL = 0;
	uint8_t OutX_H_XL = 0;
	uint8_t OutY_L_XL = 0;
	uint8_t OutY_H_XL = 0;
	uint8_t OutZ_L_XL = 0;
	uint8_t OutZ_H_XL = 0;
	int16_t Raw_X_acc = 0;
	int16_t Raw_Y_acc = 0;
	int16_t Raw_Z_acc = 0;

	float Acceleration_X = 0;
	float Acceleration_Y = 0;
	float Acceleration_Z = 0;

	LSM6DSL_Get_config();

	//Wait for acceleration data to be ready
	do {
		HAL_I2C_Mem_Read(&hi2c1, LSM6DSL_ADDRESS, LSM6DSL_STATUS_REG, 1, &LSM6DSL_STATUS, 1, 10);	
	} while ((LSM6DSL_STATUS & LSM6DS0_STATUS_REG_XLDA) == 0);

	//Read X,Y and Z registers
	HAL_I2C_Mem_Read(&hi2c1, LSM6DSL_ADDRESS, LSM6DSL_OUTX_L_XL, 1, &OutX_L_XL, 1, 10);	
	HAL_I2C_Mem_Read(&hi2c1, LSM6DSL_ADDRESS, LSM6DSL_OUTX_H_XL, 1, &OutX_H_XL, 1, 10);

	HAL_I2C_Mem_Read(&hi2c1, LSM6DSL_ADDRESS, LSM6DSL_OUTY_L_XL, 1, &OutY_L_XL, 1, 10);	
	HAL_I2C_Mem_Read(&hi2c1, LSM6DSL_ADDRESS, LSM6DSL_OUTY_H_XL, 1, &OutY_H_XL, 1, 10);

	HAL_I2C_Mem_Read(&hi2c1, LSM6DSL_ADDRESS, LSM6DSL_OUTZ_L_XL, 1, &OutZ_L_XL, 1, 10);	
	HAL_I2C_Mem_Read(&hi2c1, LSM6DSL_ADDRESS, LSM6DSL_OUTZ_H_XL, 1, &OutZ_H_XL, 1, 10);

	Raw_X_acc = ((OutX_H_XL << 8) | OutX_L_XL);
	Raw_Y_acc = ((OutY_H_XL << 8) | OutY_L_XL);
	Raw_Z_acc = ((OutZ_H_XL << 8) | OutZ_L_XL);

	Acceleration_X = (float)Raw_X_acc * acc_sensitivity;
	Acceleration_Y = (float)Raw_Y_acc * acc_sensitivity;
	Acceleration_Z = (float)Raw_Z_acc * acc_sensitivity;

	*array_data_acc = Acceleration_X;
	*(array_data_acc+1) = Acceleration_Y;
	*(array_data_acc+2) = Acceleration_Z;
}

void LSM6DSL_Get_Gyro(float *array_data_gyro)
{
	uint8_t LSM6DSL_STATUS = 0;	

	uint8_t OutX_L_G = 0;
	uint8_t OutX_H_G = 0;
	uint8_t OutY_L_G = 0;
	uint8_t OutY_H_G = 0;
	uint8_t OutZ_L_G = 0;
	uint8_t OutZ_H_G = 0;
	int16_t Raw_X_gyro = 0;
	int16_t Raw_Y_gyro = 0;
	int16_t Raw_Z_gyro = 0;

	float Gyro_X = 0;
	float Gyro_Y = 0;
	float Gyro_Z = 0;

	LSM6DSL_Get_config();

	//Wait for acceleration data to be ready
	do {
		HAL_I2C_Mem_Read(&hi2c1, LSM6DSL_ADDRESS, LSM6DSL_STATUS_REG, 1, &LSM6DSL_STATUS, 1, 10);	
	} while ((LSM6DSL_STATUS & LSM6DS0_STATUS_REG_GDA) == 0);

	//Read X,Y and Z registers
	HAL_I2C_Mem_Read(&hi2c1, LSM6DSL_ADDRESS, LSM6DSL_OUTX_L_G, 1, &OutX_L_G, 1, 10);	
	HAL_I2C_Mem_Read(&hi2c1, LSM6DSL_ADDRESS, LSM6DSL_OUTX_H_G, 1, &OutX_H_G, 1, 10);

	HAL_I2C_Mem_Read(&hi2c1, LSM6DSL_ADDRESS, LSM6DSL_OUTY_L_G, 1, &OutY_L_G, 1, 10);	
	HAL_I2C_Mem_Read(&hi2c1, LSM6DSL_ADDRESS, LSM6DSL_OUTY_H_G, 1, &OutY_H_G, 1, 10);

	HAL_I2C_Mem_Read(&hi2c1, LSM6DSL_ADDRESS, LSM6DSL_OUTZ_L_G, 1, &OutZ_L_G, 1, 10);	
	HAL_I2C_Mem_Read(&hi2c1, LSM6DSL_ADDRESS, LSM6DSL_OUTZ_H_G, 1, &OutZ_H_G, 1, 10);

	Raw_X_gyro = ((OutX_H_G << 8) | OutX_L_G);
	Raw_Y_gyro = ((OutY_H_G << 8) | OutY_L_G);
	Raw_Z_gyro = ((OutZ_H_G << 8) | OutZ_L_G);

	Gyro_X = (float)Raw_X_gyro * gyro_sensitivity;
	Gyro_Y = (float)Raw_Y_gyro * gyro_sensitivity;
	Gyro_Z = (float)Raw_Z_gyro * gyro_sensitivity;

	*array_data_gyro = Gyro_X;
	*(array_data_gyro + 1) = Gyro_Y;
	*(array_data_gyro + 2) = Gyro_Z;
}
