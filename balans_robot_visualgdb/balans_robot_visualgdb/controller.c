#include "controller.h"
#include "math.h"
#include "main.h"
#include "stm32f4xx_hal.h"
#include "model_param.h"

float K_d[4] = { -44.21, -9.99, -2.49, -4.88 }; //{-23.4460, -3.1470, -0.0933, -0.5969};

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


