#pragma once

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HTS221_h
#define __HTS221_h

#define HTS221_ADDRESS 0xBE

#define HTS221_DEV_ID 0xBC
#define HTS221_WHO_AM_I 0x0F
#define HTS221_AV_CONF 0x10
#define HTS221_CTRL_REG1 0x20
#define HTS221_CTRL_REG2 0x21
#define HTS221_STATUS_REG 0x27
#define HTS221_TEMP_OUT_H 0x2B
#define HTS221_TEMP_OUT_L 0x2A
#define HTS221_CTRL_REG2_ONE_SHOT 0x01
#define HTS221_STATUS_REG_TDA 0b00000001
#define	HTS221_H0_rH_x2						0x30					//Humidity Calibration
#define HTS221_H1_rH_x2						0x31					//Humidity Calibration
#define HTS221_T0_degC_x8					0x32					//Temperature Calibration lower
#define HTS221_T1_degC_x8					0x33					//Temperature Calibration upper
#define HTS221_T1_T0_Msb					0x35					//Temperature Calibration MSB bits since a 10-bit number
#define HTS221_H0_T0_OUT_L 			0x36					//Humidity Calibration
#define HTS221_H0_T0_OUT_H			0x37					//Humidity Calibration
#define HTS221_H1_T0_OUT_L			0x3A					//Humidity Calibration
#define HTS221_H1_T0_OUT_H			0x3B					//Humidity Calibration
#define HTS221_T0_OUT_L	0x3C					//Temperatuer Calibration
#define HTS221_T0_OUT_H	0x3D					//Temperature Calibration
#define HTS221_T1_OUT_L	0x3E					//Temperature Calibration
#define HTS221_T1_OUT_H	0x3F	


//Function prototypes


#endif