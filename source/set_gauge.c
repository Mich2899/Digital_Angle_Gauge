/************************************************************************************************
 * set_gauge.c
 * Brief : The current file contains functions calibrate,
 * 		   set_angle, set_pitch and Touch sensor interrupt handler
 * Author: Michelle Christian
 * **********************************************************************************************
 */

/*****************************************INCLUDES**************************************************/
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "MKL25Z4.h"
#include "systick.h"
#include "UART.h"
#include "mma8451.h"
#include "delay.h"
#include "TPM.h"
#include "slider.h"
#include "set_gauge.h"

/*****************************************MACROS****************************************************/
#define TOUCH_OFFSET 450  					// offset value to be subtracted - calculated on calibration
#define TOUCH_DATA (TSI0->DATA & 0xFFFF)	//macro for extracting the count from data register
#define SYSTICK_7s 3500						//Systick ticks every 20ms

/************************************GLOBAL VARIABLES************************************************/
volatile uint32_t touch_val=0; 				// declare and initialise touch value to 0

int cal_flag=0, cal_flag1=0;										//calibration flag for setting angle

float roll_cal=0.0, pitch_cal=0.0, difference=0.0;					 //roll_cal is the user defined position value of roll
																	//pitch_cal is the user defined position value of pitch
																	//difference is the difference between user defined reference and the original position for setting angle


/***********************************FUNCTION DEFINITIONS*********************************************/
//check set_gauge.h for brief
void calibrate()
{
	touch_val=0;													//touch_value reinitialized to zero
	printf("\n\rCalibrating the device.....\n\r");
	printf("Touch the slider in the middle to set reference\n\r");
    while(touch_val<500)											//until touch detected
    {
  	  TSI0->DATA |= TSI_DATA_SWTS_MASK;								//software read enabled
	  read_full_xyz();												//simultaneously read roll and pitch value
	  convert_xyz_to_roll_pitch();
    }

	  roll_cal=roll;												//once touch detected set the reference roll value
	  pitch_cal=pitch;												//once touch detected set the reference pitch value

	  printf("ACCELEROMETER CALIBRATION DONE\n\r");
	  touch_val=0;													//reset the touch variable for new input
}

//check set_gauge.h for brief
void set_angle(float angle)
{
	printf("\n\rPlease rotate the accelerometer over y-axis to achieve required angle\n\r");
	while(!(cal_flag)){												//until angle is met
        read_full_xyz();											//read current position of accelerometer
	    convert_xyz_to_roll_pitch();

	    difference=roll-roll_cal;									//difference in roll angle from user set reference
	    if(difference<-180)											//if the roll value is negative
	    {
	    	difference+=360;
	    }

	    if(difference>180)											//if the roll_call value is negative and angle increases from 180
	    {
	    	difference-=360;
	    }

	    printf("Current angle: %f\n\r",difference);					//print current value of angle for user

	    if(fabs(difference-angle)<=1)								//if the current position and
	    															//the angle to be met has a differene value less than 1
	    {
	      cal_flag=1;												//angle is met hence flag set
	  	  printf("\n\rANGLE SET\n\r");								//inform the user that angle is met
     	  color(0,255,0);											//turn on green led
	    }
	}
	cal_flag=0;														//flag reset to 0 for new input angle
}

//check set_gauge.h for brief
void set_pitch(float horiz_angle)
{
	printf("\n\rPlease rotate the accelerometer over x-axis to achieve required angle\n\r");
	while(!(cal_flag1)){											//until angle is met
        read_full_xyz();											//read current position of accelerometer
	    convert_xyz_to_roll_pitch();

	    printf("Current pitch: %f\n\r", pitch);

	    if(fabs(pitch-horiz_angle)<=1)						//if the current position and
																	//the angle to be met has a differene value less than 1
	    {
	      cal_flag1=1;												//pitch is met hence flag set
	  	  printf("PITCH SET\n\r");									//inform the user that angle is met
     	  color(0,0,255);											//turn on blue led
	    }
	}
	cal_flag1=0;													//flag reset to 0 for new input pitch
}

//check set_gauge.h for brief
void test_TSI()
{
	printf("\n\rTouch sensor test for 7 seconds!!\n\r");
	printf("Touch on left side of the slider to see red led glow up\n\r");
	printf("Touch on center of the slider to see green led glow up\n\r");
	printf("Touch on right side of the slider to see blue led glow up\n\r\n\r");
	while(get_timer()<SYSTICK_7s)												//run the test for 7 seconds
	{
	  	 TSI0->DATA |= TSI_DATA_SWTS_MASK;								//software read enabled
	  	 if(touch_val>200 && touch_val<400){							//value range for left
	  		 color(255,0,0);
	  	 }
	  	 else if(touch_val>400 && touch_val<600){						//value range for center
	  		 color(0,255,0);
	  	 }
	  	 else if(touch_val>600){										//value range for right
	  		 color(0,0,255);
	  	 }
	  	 else															//turn off leds if no touch detected
	  	 {
	  		 color(0,0,0);
	  	 }
	}
	touch_val=0;														//reset the touch variable for touch_offset
	color(0,0,0);														//turn off leds if any on
	reset_timer();														//reset timer for bubble level demo
}

//check set_gauge.h for brief
void TSI0_IRQHandler (void)
{
	for(int d=0;d<1000;d++);
	touch_val = TOUCH_DATA - TOUCH_OFFSET;	//calculate touch_val
	TSI0->GENCS |= TSI_GENCS_EOSF_MASK ; 	//writing one to clear the flag

}
