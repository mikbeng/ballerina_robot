#include "controller.h"
#include "math.h"
#include "main.h"
#include "stm32f4xx_hal.h"
#include "model_param.h"

float Kp = 20;
float Kd = 70;
float Ki = 5;	

float K_d[4] = { -17.00, -3.71, -0.00, -0.06 };           //{-23.4460, -3.1470, -0.0933, -0.5969};

float T_motors = 0;

float calc_control_input()
{
	float u = 0;
	float ang_acc_motors = 0;
	
	u = (-K_d[0]*state_vector[0]) + (-K_d[1]*state_vector[1]) + (-K_d[2]*state_vector[2]) + (-K_d[3]*state_vector[3]);
	
	T_motors = u*(R_WHEEL / 2);
	ang_acc_motors = T_motors / I_WHEEL;
	
	return ang_acc_motors;
}

float PID()
{
	float e = 0;
	static float u = 0;
	float delta_u = 0;
	static float e1, e2, u1 = 0;
	
	
	e = state_vector[0];
	
	delta_u = (Kp*(e - e1)) + (Ki*TS*e) + ((Kd / TS) * (e - (2*e1) + e2));

	u = u + delta_u;

	e2 = e1;

	e1 = e;

	u1 = u;
	
	return u;
}
