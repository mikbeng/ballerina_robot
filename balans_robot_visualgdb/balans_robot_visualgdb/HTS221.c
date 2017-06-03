#include "i2c.h"
#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "HTS221.h"

//Calibration variables, global for HTS221.c
float T0_OUT = 0;
float T1_OUT = 0;
float T0_DegC = 0;
float T1_DegC = 0;

void HTS221_init(void)
{
	uint8_t signature;

	//Calibration variables - local
	int8_t T0_OUT_L = 0;
	int8_t T0_OUT_H = 0;
	int8_t T1_OUT_L = 0;
	int8_t T1_OUT_H = 0;
	uint8_t T0_degC_x8 = 0;
	uint8_t T1_degC_x8 = 0;
	uint8_t Msb_TO_T1_degC = 0;
	
	uint8_t ctrl_reg1_conf = 0b10000100;

	HAL_I2C_Mem_Read(&hi2c1, HTS221_ADDRESS, HTS221_WHO_AM_I, 1, &signature, 1, 100);	//Get device signature
	
	if (signature == HTS221_DEV_ID)
	{
		//Config device PD>>1 and BDU>>1 in ctrl_reg1
		HAL_I2C_Mem_Write(&hi2c1, HTS221_ADDRESS, HTS221_CTRL_REG1, 1, &ctrl_reg1_conf, 1, 10);	
		
		//Calibration - temperature
		HAL_I2C_Mem_Read(&hi2c1, HTS221_ADDRESS, HTS221_T0_OUT_L, 1, &T0_OUT_L, 1, 10);
		HAL_I2C_Mem_Read(&hi2c1, HTS221_ADDRESS, HTS221_T0_OUT_H, 1, &T0_OUT_H, 1, 10);
		HAL_I2C_Mem_Read(&hi2c1, HTS221_ADDRESS, HTS221_T1_OUT_L, 1, &T1_OUT_L, 1, 10);
		HAL_I2C_Mem_Read(&hi2c1, HTS221_ADDRESS, HTS221_T1_OUT_H, 1, &T1_OUT_H, 1, 10);
		HAL_I2C_Mem_Read(&hi2c1, HTS221_ADDRESS, HTS221_T0_degC_x8, 1, &T0_degC_x8, 1, 10);
		HAL_I2C_Mem_Read(&hi2c1, HTS221_ADDRESS, HTS221_T1_degC_x8, 1, &T1_degC_x8, 1, 10);
		HAL_I2C_Mem_Read(&hi2c1, HTS221_ADDRESS, HTS221_T1_T0_Msb, 1, &Msb_TO_T1_degC, 1, 10);
		//Process Calibration Registers
		T0_DegC = ((float)(((Msb_TO_T1_degC & 0x3) << 8) | (T0_degC_x8)) / 8.0);
		T1_DegC = ((float)(((Msb_TO_T1_degC & 0xC) << 6) | (T1_degC_x8)) / 8.0); //Value in 3rd and 4th bit so only shift 6
		T0_OUT = (float)((T0_OUT_H << 8) | T0_OUT_L);
		T1_OUT = (float)((T1_OUT_H << 8) | T1_OUT_L);
	}
	else
	{
		//Init error
	}
}

void HTS221_read_reg(void *handle, uint8_t RegAddr, uint16_t NumByteToRead, uint8_t *Data)
{
	//uint8_t reg_ptr = HTS221_WHO_AM_I;
	//uint8_t i2c_read_data_buffer[1];
	
	
	// first set the register pointer to the register wanted to be read
	HAL_I2C_Master_Transmit(handle, HTS221_ADDRESS, &RegAddr, 1, 100);  // note the & operator which gives us the address of the register_pointer variable

	HAL_I2C_Master_Receive(handle, HTS221_ADDRESS, Data, NumByteToRead, 10);

}

float HTS221_read_temp(void)
{
	uint8_t ctrl_reg2_conf = HTS221_CTRL_REG2_ONE_SHOT;

	/* Local Variables */
	uint8_t STATUS_REG = 0;
	uint8_t T_OUT_L = 0;
	uint8_t T_OUT_H = 0;
	float T_OUT = 0;
	float Temperature_In_C = 0;
	float Temperature_In_F = 0;
	
	//Start a temperature conversion
	HAL_I2C_Mem_Write(&hi2c1, HTS221_ADDRESS, HTS221_CTRL_REG2, 1, &ctrl_reg2_conf, 1, 10);	
	
	//Wait for Temperature data to be ready
	do {
		HAL_I2C_Mem_Read(&hi2c1, HTS221_ADDRESS, HTS221_STATUS_REG, 1, &STATUS_REG, 1, 10);	
	} while ((STATUS_REG & HTS221_STATUS_REG_TDA) == 0);
	
	
	//Read Temperature Data and Calibration
	HAL_I2C_Mem_Read(&hi2c1, HTS221_ADDRESS, HTS221_TEMP_OUT_L, 1, &T_OUT_L, 1, 10);	
	HAL_I2C_Mem_Read(&hi2c1, HTS221_ADDRESS, HTS221_TEMP_OUT_H, 1, &T_OUT_H, 1, 10);
	
	T_OUT =  (float)((T_OUT_H << 8) | T_OUT_L);
	
	//Calculate Temperatuer using linear interpolation and convert to Fahrenheit
	Temperature_In_C = (float)(T0_DegC + ((T_OUT - T0_OUT)*(T1_DegC - T0_DegC)) / (T1_OUT - T0_OUT));
	Temperature_In_F = (Temperature_In_C*(9.0 / 5.0)) + 32.0;
	
	return (Temperature_In_C);
	
}
