#include "math.h"
#include "main.h"
#include "stm32f4xx_hal.h"
#include "sensor.h"

#define GYROSCOPE_SENSITIVITY 8.75

//Function prototypes
void LSM6DSL_Get_Acc(float *array_data_acc);
void LSM6DSL_Get_Gyro(float *array_data_gyro);
float *get_acc_angles(void);
float *get_gyro_vel(void);
float ComplementaryFilter(float *theta, float *d_theta);

//Global variables
float *angles;
float *vel_angles;
float Acc_data[3] = { 0.0, 0.0, 0.0 };
float Gyro_data[3] = { 0.0, 0.0, 0.0 };

float theta = 0;
float d_theta = 0;

int32_t R_acc[3][3] = { { 0, -1, 0 }, { 0, 0, 1 }, { -1, 0, 0 } };
int32_t R_gyro[3][3] = { { 0, -1, 0 }, { 0, 0, -1 }, { 1, 0, 0 } };

void get_states(float *statevector, float w_deg)
{
	//theta
	angles = get_acc_angles();
	vel_angles = get_gyro_vel();
	
	theta = *angles;
	d_theta = *(vel_angles+1);
	
	*statevector = ComplementaryFilter(&theta, &d_theta);
	//d_theta
	LSM6DSL_Get_Gyro(Gyro_data);
	*(statevector+1) = -Gyro_data[0];
	
	//x
	*(statevector + 2) = 0;
	
	//dx
	*(statevector + 3) = w_deg*R_WHEEL;	
	
}

float ComplementaryFilter(float *theta, float *d_theta)
{
	static float theta_comp = 0;
	static uint8_t init_step_flag = 0;
	
	float a = 0.95;

	theta_comp = a * (theta_comp + (*d_theta * TS)) + *theta * (1 - a); 
	
	return theta_comp;
}
	
float *get_gyro_vel()
{
	uint32_t m, k = 0;
	float sum = 0;
	float R_gyro_data[3];	//(x,y,z)
	
	static float gyro_vel_ret[3] = { 0, 0, 0 };
	
	LSM6DSL_Get_Gyro(Gyro_data);
	
	//Rotations
	for(k = 0 ; k < R3 ; k++) {
		for (m = 0; m < R3; m++) {
			sum = sum + (Gyro_data[m]*R_gyro[m][k]);
		}
		R_gyro_data[k] = sum;
		sum = 0;
	}
	
	gyro_vel_ret[0] = R_gyro_data[0];
	gyro_vel_ret[1] = R_gyro_data[1];
	gyro_vel_ret[2] = R_gyro_data[2];
	
	return gyro_vel_ret;
}

float *get_acc_angles()
{
	uint32_t m, k = 0;
	float theta = 0;
	float phi = 0;
	float alpha = 0;
	
	static float angel_ret[3] = {0, 0, 0};	//(x,y,z)
	
	float sum = 0;
	float R_Acc_data[3];
	LSM6DSL_Get_Acc(Acc_data);
		
	//Rotations
	for (k = 0; k < R3; k++) {
		for (m = 0; m < R3; m++) {
			sum=sum + (Acc_data[m]*R_acc[m][k]);
			}
		R_Acc_data[k] = sum;
		sum = 0;
		}
	
	//Calculate angle from force vectors
	theta = (atan2(R_Acc_data[X_AXIS], -R_Acc_data[Z_AXIS]) * (180 / M_PI));
	//phi=...
	//alpha=...
	
	angel_ret[0] = theta;
	angel_ret[1] = phi;
	angel_ret[2] = alpha;
	
	return angel_ret;
}


