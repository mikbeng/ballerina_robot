
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __stepper_h
#define __stepper_h

//MAsks for step size
#define STEP_SIZE_FULL_MASK	0b00000000
#define STEP_SIZE_HALF_MASK	0b00000001
#define STEP_SIZE_1_4_MASK	0b00000010
#define STEP_SIZE_1_8_MASK	0b00000011
#define STEP_SIZE_1_16_MASK	0b00000100
#define STEP_SIZE_1_32_MASK	0b00000101

#define STEP_SIZE_FULL	1
#define STEP_SIZE_HALF	2
#define STEP_SIZE_1_4	4
#define STEP_SIZE_1_8	8
#define STEP_SIZE_1_16	16
#define STEP_SIZE_1_32	32

#define STEP_SIZE_USE (16)

#define V_ABS_LIM (200)


// Tangential velocity define
#define WHEEL_D (6.5f)			//wheel diameter in cm
#define STEP_SIZE (1.8f)		//1 full step size of stepper


//Function prototypes


#endif
