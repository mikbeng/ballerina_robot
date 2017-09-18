#ifndef MLX_COMMUNICATION_H
#define	MLX_COMMUNICATION_H

void mlxCommunicationInit();
unsigned short mlxGetAngle(unsigned short *angle);
void mlxPowerUp();
void mlxPowerDown();

#define ERROR_MATH_ERROR 1
#define ERROR_MLX 2
#define ERROR_MLX_INVALID 4
#define ERROR_VBAT_LOW 8

#endif	/* MLX_COMMUNICATION_H */

