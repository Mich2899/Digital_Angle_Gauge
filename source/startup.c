/*
 * *************************************************************************************************
 * startup.c
 *		This file contains two functions that run on startup of the accelerometer,
 *		init_all and bubble_demo
 *		Author: Michelle Christian
 * *************************************************************************************************
*/

/*****************************************INCLUDES**************************************************/
#include <stdio.h>
#include <math.h>
#include "i2c.h"
#include "systick.h"
#include "sysclock.h"
#include "TPM.h"
#include "slider.h"
#include "UART.h"
#include "mma8451.h"
#include "delay.h"
#include "startup.h"

/******************************************MACROS**************************************************/
#define baud_rate 115200			//Baud rate for UART set as 115200
#define SYSTICK_7s 3500				//Systick ticks every 20ms

/***************************************FUNCTION DEFINITIONS****************************************/
void init_all()
{
    i2c_init();   													//initializes i2c module
	init_systick();													//initializes systick module
    sysclock_init();												//initiliazes the sysclock module
	TPM_init();														//initializes PWM/TPM module
    initialise_touch();												//initializes touch sensor

    Init_UART0(baud_rate);											//UART initialization
	if (!init_mma()) {												/* init mma peripheral */
		color(255, 0, 0);											/* Light red error LED */
		while (1)													/* not able to initialize mma */
			;
	}
}

void bubble_demo()
{
	int r1=0, g1=0, b1=0;											//value for LED colors
	printf("Bubble level demo for 7 seconds!!\n\r");
	printf("Rotate the board at different angles to see different colors!!\n\r");
	while(get_timer()<=SYSTICK_7s){										//run bubble_demo for 7 seconds(systick ticks at every 20ms)
    	read_full_xyz();											//read current position of accelerometer
    	convert_xyz_to_roll_pitch();
		r1=50;														//set r to some specific value
		g1=fabs(roll);												//set value of green LED according to roll angle
		b1=fabs(pitch);												//set value of blue LED according to pitch angle
		color(r1,g1,b1);											//set color using TPM functionalities
	}
	Delay(50);														//delay 50 ms
	color(0,0,0);													//turn off all leds
	reset_timer();													//reset the timer
}
