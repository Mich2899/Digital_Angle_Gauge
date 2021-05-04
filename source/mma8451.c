/**
 * *************************************************************************************************
 * @file    mma8451.c
 * @brief   This file containss functions for mma8451 in built accelerometer
 * 			initialization, read_full_xyz, read_xyz, and convert_xyz_to_roll_pitch
 *
 * @author  Michelle Christian
 * *************************************************************************************************
 */

/*****************************************INCLUDES**************************************************/
#include <MKL25Z4.H>
#include "mma8451.h"
#include "i2c.h"
#include "delay.h"
#include <math.h>

/******************************************MACROS**************************************************/
#define MMA_ADDR 0x3A					//accelerometer device address

#define REG_XHI 0x01					//register value for X axis high value
#define REG_XLO 0x02					//register value for X axis low value
#define REG_YHI 0x03					//register value for Y axis high value
#define REG_YLO 0x04					//register value for Y axis low value
#define REG_ZHI	0x05					//register value for Z axis high value
#define REG_ZLO 0x06					//register value for Z axis low value

#define REG_CTRL1  0x2A					//register address for accelerometer

#define COUNTS_PER_G (4096.0)			//counts per gravitational constant value
#define M_PI (3.14159265)				//pi value to convert axis into angle

/***************************************GLOBAL VARIABLES******************************************/
int16_t acc_X=0, acc_Y=0, acc_Z=0;		//variables for x, y and z position
float roll=0.0, pitch=0.0;				//roll angle and pitch angle


/***************************************FUNCTION DEFINITIONS****************************************/
//check mma8451.h for brief
int init_mma()
{
	//set active mode, 14 bit samples and 800 Hz ODR
	i2c_write_byte(MMA_ADDR, REG_CTRL1, 0x01);
	return 1;
}

//check mma8451.h for brief
void read_full_xyz()
{
	int i;
	uint8_t data[6];					//array to read data for axis
	int16_t temp[3];					//stores values for all three axis

	i2c_start();						//sends start bit
	i2c_read_setup(MMA_ADDR , REG_XHI);	//prepares the i2c module for read from the device by setting up the
										//device address and register address

	// Read five bytes in repeated mode
	for( i=0; i<5; i++)	{
		data[i] = i2c_repeated_read(0);
	}
	// Read last byte ending repeated mode
	data[i] = i2c_repeated_read(1);

	for ( i=0; i<3; i++ ) {
		temp[i] = (int16_t) ((data[2*i]<<8) | data[2*i+1]);
	}

	// Align for 14 bits
	acc_X = temp[0]/4;
	acc_Y = temp[1]/4;
	acc_Z = temp[2]/4;
}

//check mma8451.h for brief
void read_xyz(void)
{
	// sign extend byte to 16 bits - need to cast to signed since function
	// returns uint8_t which is unsigned
	acc_X = (int8_t) i2c_read_byte(MMA_ADDR, REG_XHI);
	Delay(100);
	acc_Y = (int8_t) i2c_read_byte(MMA_ADDR, REG_YHI);
	Delay(100);
	acc_Z = (int8_t) i2c_read_byte(MMA_ADDR, REG_ZHI);

}

//check mma8451.h for brief
void convert_xyz_to_roll_pitch(void) {
	//divide each axis by counts for gravitational constant
	float ax = acc_X/COUNTS_PER_G,
				ay = acc_Y/COUNTS_PER_G,
				az = acc_Z/COUNTS_PER_G;

	roll = atan2(ay, az)*180/M_PI;						//calculate roll angle
	pitch = atan2(ax, sqrt(ay*ay + az*az))*180/M_PI;	//calculate pitch angle

}
