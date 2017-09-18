#include "MLX_calc.h"
#include "mlx_communication.h"
#include "gpio.h"
#include "main.h"
#include "tim.h"

enum operationalModeEnum operationalMode=OpModeRun;


/*
 * This function runs the length measurement system and callibration
 */
void runLengthMeasureSystem()
{
	uint16_t angle = 0;
	uint32_t errCode;
	errCode = mlxGetAngle(&angle);

	updateQuadrantCountFromAngle(angle, &quadrantCount, &currentQuadrant);
	float localLength;
	errCode = stateToLength(&localLength, quadrantCount, angle);

	if (operationalMode == OpModeRun)
	{
		length = localLength - lengthOffset;
	}
	else if (operationalMode == OpModeCallib)
	{
		if (localLength < lengthOffset)
			lengthOffset = localLength;
		if ((localLength - lengthOffset) > maxLength)
			maxLength = (localLength - lengthOffset);
	}
}

/*
 * This function takes the quadrant count and angle (0-((2^14)-1) and sets the length and error variables accordingly
 * It does this by calculating a coarse length and a fine length and adds them together
 */
uint8_t stateToLength(float *length, int32_t quadrantCount, uint16_t angle) {
	//Variables
	float coarseLength;
	float fineLengthAngle;
	float fineLength;
	float angleOffset;
	uint8_t error = 0;
	uint8_t currentQuadrant = angle >> 12;

	//Constants
	const int32_t angle90deg = 4096;
	const int32_t angle360deg = 4096 * 4;

	coarseLength =  ((quadrantCount * drumCircumference)) / 4;

	angleOffset = currentQuadrant * angle90deg + (angle90deg >> 1); //This is the angle in the center of the current quadrant.

	if (angle > 0 && angle < angle90deg && currentQuadrant == 3)
		angle += angle360deg;

	fineLengthAngle = (angle - angleOffset); //Angle difference for fine length
	fineLength =  (fineLengthAngle * drumCircumference) / 16384; //Calculate fine length

	if (fineLengthAngle > angle90deg || fineLengthAngle < -angle90deg)//Something is wrong if the quadrant count and angle does not agree.
		error |= ERROR_MATH_ERROR;

	*length = (coarseLength + fineLength);

	return error;
}

float calcSpeed(float length) {
	static float lastLength = 0;
	float speed;
	float deltaLength;
	float deltaTime;

	//StopTimer
	HAL_TIM_Base_Stop(&htim3);
	
	//Recalc speed
	deltaTime = __HAL_TIM_GET_COUNTER(&htim3) / 100000.0;//In s
	deltaLength = (length - lastLength) / 1000;
	lastLength = length;

	speed = deltaLength / deltaTime;

	//Restart timer
	__HAL_TIM_SET_COUNTER(&htim3, 0);
	HAL_TIM_Base_Start(&htim3);
	return speed;
}


/*
 * Updates the quadrantCount based on a measured angle
 */
void updateQuadrantCountFromAngle(uint16_t angle, int32_t *quadrantCount, uint8_t *currentQuadrant) {
	uint8_t newQuadrant;
	newQuadrant = angle >> 12;

	if (((newQuadrant + 3) % 4) == *currentQuadrant)
		(*quadrantCount)++;
	else if (((newQuadrant + 1) % 4) == *currentQuadrant)
		(*quadrantCount)--;

	*currentQuadrant = newQuadrant;
}

