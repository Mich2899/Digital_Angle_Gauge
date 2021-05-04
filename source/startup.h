/*
 * **********************************************************************************************
 * startup.h
 *		This file explains functions that run on startup
 *		initialization of all the modules and bubble level demo
 *	@author: Michelle Christian
 * **********************************************************************************************
 */

#ifndef STARTUP_H_
#define STARTUP_H_

/*
 * function		: init_all
 * params		: void/none
 * brief		: initializes i2c, systick, sysclock, PWM and touch modules
 * return_type	: void
 */
void init_all();

/*
 * function		: bubble_demo
 * params		: void/none
 * brief		: demonstrates bubble_demo hence testing the accelerometer
 * return_type	: void
 */
void bubble_demo();

#endif /* STARTUP_H_ */
