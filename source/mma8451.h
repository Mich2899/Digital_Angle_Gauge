/**
 * **********************************************************************************************
 * @file    mma8451.h
 * @brief   This file explains functions for mma8451 in built accelerometer
 * 			initialization, read_full_xyz, read_xyz, and convert_xyz_to_roll_pitch
 *
 * 			NOTE: There are 5 variables declared as extern as they are used in menu and command as
 * 			well as startup files
 * @author  Michelle Christian
 * **********************************************************************************************
 */
#ifndef MMA8451_H
#define MMA8451_H
#include <stdint.h>

extern float roll, pitch;				//roll(rotation angle over y axis)
										//pitch(rotation angle over x axis)
extern int16_t acc_X, acc_Y, acc_Z;		//accelerometer position values for X, Y and Z axis

/*
 * function		: init_mma
 * params		: void/none
 * brief		: initializes mma8451
 * return_type	: void
 */
int init_mma(void);

/*
 * function		: read_full_xyz
 * params		: void/none
 * brief		: uses i2c functionalities to read values from the accelerometer
 * 				  reads the values of X, Y and Z axis
 * return_type	: void
 */
void read_full_xyz(void);

/*
 * function		: read_xyz
 * params		: void/none
 * brief		: uses i2c functionalities to read values from the accelerometer at a delay of 100ms
 * 				  reads the values of X, Y and Z axis
 * return_type	: void
 */
void read_xyz(void);

/*
 * function		: convert_xyz_to_roll_pitch
 * params		: void/none
 * brief		: converts the value obtained in x, y and z axis into roll(rotation over y axis)
 * 				  and pitch(rotation over x axis)
 * return_type	: void
 */
void convert_xyz_to_roll_pitch(void);

#endif
