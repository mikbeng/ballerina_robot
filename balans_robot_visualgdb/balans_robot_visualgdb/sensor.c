#include "math.h"
#include "main.h"
#include "stm32f4xx_hal.h"

#define dt 0.01
#define GYROSCOPE_SENSITIVITY 8.75

void LSM6DSL_Get_Acc(float *array_data_acc);
void LSM6DSL_Get_Gyro(float *array_data_gyro);
float get_theta_raw();

float Acc_data[3] = { 0.0, 0.0, 0.0 };
float Gyro_data[3] = { 0.0, 0.0, 0.0 };
float angle_raw_acc = 0;
float gyro_pitch = 0;


void get_states(float *statevector, float w_rad)
{
	//theta
	*statevector=get_theta_raw();
	
	//d_theta
	LSM6DSL_Get_Gyro(Gyro_data);
	*(statevector+1) = -Gyro_data[0];
	
	//x
	*(statevector + 2) = 0;
	
	//dx
	*(statevector + 3) = w_rad;
	
}

void ComplementaryFilter(float *theta_comp, float *theta_raw)
{
	static uint8_t init_step_flag = 0;
	LSM6DSL_Get_Acc(Acc_data);
	LSM6DSL_Get_Gyro(Gyro_data);
	
	gyro_pitch = gyro_pitch + (-Gyro_data[0] * dt);		// Forward Euler. Angle around x-axis

	angle_raw_acc = (atan(Acc_data[2] / Acc_data[1]) * (180 / M_PI));
	*theta_raw = angle_raw_acc;
	*theta_comp = gyro_pitch * 0.98 + angle_raw_acc * 0.02;
}

float get_theta_raw()
{
	float theta_raw;
	LSM6DSL_Get_Acc(Acc_data);
	theta_raw = (atan(Acc_data[2] / Acc_data[1]) * (180 / M_PI));
	
	return theta_raw;
}

