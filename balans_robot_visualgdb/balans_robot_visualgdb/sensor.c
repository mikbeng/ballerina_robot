#include "math.h"
#include "main.h"
#include "stm32f4xx_hal.h"

#define dt 0.01
#define GYROSCOPE_SENSITIVITY 8.75

void LSM6DSL_Get_Acc(float *array_data_acc);
void LSM6DSL_Get_Gyro(float *array_data_gyro);
float get_theta_raw();
void ComplementaryFilter(float *theta_comp);
float theta_comp = 0;
float theta_raw = 0;
float Acc_data[3] = { 0.0, 0.0, 0.0 };
float Gyro_data[3] = { 0.0, 0.0, 0.0 };
float angle_raw_acc = 0;
float gyro_pitch = 0;


void get_states(float *statevector, float w_rad)
{
	//theta
	theta_raw = get_theta_raw();
	ComplementaryFilter(statevector);
	//d_theta
	LSM6DSL_Get_Gyro(Gyro_data);
	*(statevector+1) = -Gyro_data[0];
	
	//x
	*(statevector + 2) = 0;
	
	//dx
	*(statevector + 3) = w_rad*R_WHEEL;	//WRONG!
	
}

void ComplementaryFilter(float *theta_comp)
{
	static uint8_t init_step_flag = 0;
	LSM6DSL_Get_Acc(Acc_data);
	LSM6DSL_Get_Gyro(Gyro_data);
	
	float a = 0.98;
	
	/*
	gyro_pitch = gyro_pitch + (-Gyro_data[0] * dt);		// Forward Euler. Angle around x-axis

	angle_raw_acc = (atan(Acc_data[2] / Acc_data[1]) * (180 / M_PI));
	*theta_comp = gyro_pitch * 0.98 + angle_raw_acc * 0.02;*/
	
	angle_raw_acc = (atan(Acc_data[2] / Acc_data[1]) * (180 / M_PI));
	*theta_comp = a * (*theta_comp + (-Gyro_data[0] * dt)) + angle_raw_acc * (1-a); 
	
}

float get_theta_raw()
{
	float theta_raw;
	LSM6DSL_Get_Acc(Acc_data);
	theta_raw = (atan(Acc_data[2] / Acc_data[1]) * (180 / M_PI));
	
	return theta_raw;
}

