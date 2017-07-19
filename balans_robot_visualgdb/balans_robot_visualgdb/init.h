/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __init_h
#define __init_h

#include "stdio.h"

//Function prototypes
void get_clock_status(void);
void HTS221_init(void);
void LSM6DSL_init(void);
void stepper_setstep(uint32_t step_size_used, uint32_t stepper_val);

#endif