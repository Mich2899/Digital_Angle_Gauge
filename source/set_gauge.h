/*
 * **********************************************************************************************
 * set_gauge.h
 * Brief : The current file explains functions such as calibrate,
 * 		   set_angle, set_pitch and Touch sensor interrupt handler
 *
 * 			NOTE: There are 2 variables declared as extern as they are used in menu and command file
 * Author: Michelle Christian
 * **********************************************************************************************
 */

#ifndef SET_GAUGE_H_
#define SET_GAUGE_H_


extern float roll_cal, pitch_cal;	//roll_cal(user defined position value of rotation angle over y axis)
									//pitch_cal(user defined position value of rotation angle over x axis)

/*
 * function		: calibrate
 * params		: void/none
 * brief		: calibrates the accelerometer according to user defined reference
 * 				  Touch sensor is used to define the new reference
 * return_type	: void
 */
void calibrate();

/*
 * function		: set_angle
 * params		: float angle
 * brief		: sets the roll angle user wants it to set to and informs the user on terminal once set
 * return_type	: void
 */
void set_angle(float angle);

/*
 * function		: set_pitch
 * params		: float horiz_angle
 * brief		: sets the pitch angle user wants it to set to and informs the user on terminal once set
 * return_type	: void
 */
void set_pitch(float horiz_angle);

/*
 * function		: TSI0_IRQHandler
 * params		: void/none
 * brief		: interrupt handler for touch slider
 * return_type	: void
 */
void TSI0_IRQHandler (void);

/*
 * function		: test_TSI
 * params		: void/none
 * brief		: test for capacitive touch input
 * return_type	: void
 */
void test_TSI();

#endif /* SET_GAUGE_H_ */
