
#include "gpio.h"
#include "mlx_communication.h"
#include "main.h"

#define SCLK_LOW            HAL_GPIO_WritePin(MLX1_SCLK_GPIO_Port,MLX1_SCLK_Pin,0);
#define SCLK_HIGH           HAL_GPIO_WritePin(MLX1_SCLK_GPIO_Port,MLX1_SCLK_Pin,1);
#define SET_MISO_MOSI(x)    if(x){HAL_GPIO_WritePin(MLX1_MOSI_MISO_GPIO_Port,MLX1_MOSI_MISO_Pin,1);}else{HAL_GPIO_WritePin(MLX1_MOSI_MISO_GPIO_Port,MLX1_MOSI_MISO_Pin,0);}
#define GET_MISO_MOSI       HAL_GPIO_ReadPin(MLX1_MOSI_MISO_GPIO_Port,MLX1_MOSI_MISO_Pin)
#define SS_LOW              HAL_GPIO_WritePin(MLX1_CS_GPIO_Port,MLX1_CS_Pin,0);
#define SS_HIGH             HAL_GPIO_WritePin(MLX1_CS_GPIO_Port,MLX1_CS_Pin,1);
#define PULLUP_ENABLE       HAL_GPIO_WritePin(MLX1_PULL_GPIO_Port,MLX1_PULL_Pin,1);
#define PULLUP_DISABLE      HAL_GPIO_WritePin(MLX1_PULL_GPIO_Port,MLX1_PULL_Pin,0);


uint64_t systemTime = 0;
float length = 0, speed = 0;
int32_t quadrantCount = 0, quadrantCount2 = 0;
uint8_t currentQuadrant, currentQuadrant2;
float lengthOffset = 0;
float maxLength = 0;
uint8_t batteryStatus = 100;
float GVNPressure = 0;//Pressure in BAR 0 -> -1

void mlxCommunicationInit() {

	mlxPowerUp();
	uint16_t timeOut=1000,dummy;
	while(timeOut-- && mlxGetAngle(&dummy));
}

static void sendByte(unsigned char byte) {
	unsigned char i;
	
	SCLK_LOW;
	for (i = 0; i < 8; i++) {
		SCLK_HIGH;
		SCLK_HIGH;
		SCLK_HIGH;
		SET_MISO_MOSI((byte & 0x80) ? 1 : 0);
		byte <<= 1;
		SCLK_LOW;
		SCLK_LOW;
		SCLK_LOW;
		SCLK_LOW;
	}
	SET_MISO_MOSI(1);
}

static unsigned char getByte(void) {
	unsigned char i, byte = 0;

	SCLK_LOW;
	for (i = 0; i < 8; i++) {
		SCLK_HIGH;
		SCLK_HIGH;
		SCLK_HIGH;
		byte <<= 1;
		byte |= (GET_MISO_MOSI) ? 1 : 0;
		SCLK_LOW;
		SCLK_LOW;
		SCLK_LOW;
		SCLK_LOW;
	}
	return byte;
}

static unsigned short getData() {
	unsigned short data, dataInv,i;
	SS_HIGH;
	SS_LOW;

	for(i=0;i<200;i++);//Dummy loop to delay
	sendByte(0xAA);
	sendByte(0xFF);
	for(i=0;i<100;i++);//Dummy loop to delay
	data = getByte() << 8;
	for(i=0;i<50;i++);//Dummy loop to delay
	data |= getByte();
	for(i=0;i<50;i++);//Dummy loop to delay
	dataInv = getByte() << 8;
	for(i=0;i<50;i++);//Dummy loop to delay
	dataInv |= getByte();
	for(i=0;i<50;i++);//Dummy loop to delay

	sendByte(0xFF);
	sendByte(0xFF);
	sendByte(0xFF);
	sendByte(0xFF);
	for(i=0;i<5;i++);//Dummy loop to delay
	SS_HIGH;
	dataInv=~dataInv;
	if (data==dataInv)
		return data;
	return 0;
}

/*
 * return status:z
 * 0: all ok
 * 1: communication fault
 * >1: See mlx90316 datasheet
 */
unsigned short mlxGetAngle(unsigned short *angle) {
	unsigned short response;
	unsigned char retries = 1;
	static uint16_t errors=0,correct=0;


	do {
		response = getData();
	} while (--retries && !response);
	switch (response & 0b11) {
	case 0:
		errors++;
		return ERROR_MLX_INVALID;
	case 1:
		correct++;
		*angle = 16383 - (response >> 2);
		return 0;
	case 2:
		errors++;
		return ERROR_MLX;
	case 3:
		errors++;
		return ERROR_MLX_INVALID;
	}
	return 1;
}

void mlxPowerUp() {
	SS_HIGH;
	SCLK_LOW;
	PULLUP_ENABLE;

}

void mlxPowerDown() {

	SS_LOW;
	SCLK_LOW;
	SET_MISO_MOSI(0);
	PULLUP_DISABLE;
}
