#pragma once
#ifndef __MLX_calc_H_
#define __MLX_calc_H_
#include "stdint.h"
#include "stdbool.h"


#define drumCircumference   238.76104

enum operationalModeEnum
{
	OpModeRun    = 0,
	OpModeCallib
};

extern enum operationalModeEnum operationalMode;


void runLengthMeasureSystem();
void updateQuadrantCountFromAngle(uint16_t angle, int32_t *quadrantCount, uint8_t *currentQuadrant);
uint8_t stateToLength(float *length, int32_t quadrantCount, uint16_t angle);
float calcSpeed(float length);

#endif
