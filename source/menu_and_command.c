/*
 * **********************************************************************************************
 * menu_and_command.c
 * brief	:	contains menu, command processor and check functions.
 * 				menu provides options to the user
 * 				command processor processes those options to provide appropriate output
 * @author	:   Michelle Christian
 * **********************************************************************************************
 */

/*****************************************INCLUDES**************************************************/
#include <stdio.h>
#include <math.h>
#include <menu_and_command.h>
#include <set_gauge.h>
#include <string.h>
#include "UART.h"
#include "mma8451.h"
#include "delay.h"
#include "TPM.h"

/******************************************MACROS**************************************************/
#define CHK_BCKSP (8)											//(8 is the decimal value for backspace)


/***************************************GLOBAL VARIABLES******************************************/
int flag=0, ret1=1, ret2=1, ret3=1;								//flag to check if user recalibrated or provided invalid input
																//return variable to compare 'calibrate' string input from user
																//return variable to compare 'angle<value>' string input from user
																//return variable to compare 'pitch<value>' string input from user

float input_pitch=0.0, input_angle=0.0;							//input pitch and angle provided by user
char input[256], input_ang[10], input_pit[10];					//input is the buffer where we take input command from user
																//input_ang is when user enters angle<value> format
																//input_pitch is when user enters pitch<value> format


/***************************************FUNCTION DEFINITIONS****************************************/
//check menu_and_command.h for brief
void blink(int r2, int g2, int b2)
{
	for(int i=0;i<3;i++){
	  color(0,0,0);												//turn off leds
	  Delay(100);												//delay for 100ms
	  color(r2,g2,b2);											//turn on leds
	  Delay(100);												//delay for 100ms
	  color(0,0,0);												//turn off leds
	}
}

//check menu_and_command.h for brief
void command_processor()
{
	  ret1=strcmp(input,"calibrate");								//ret1 stores return value of string comparison with "calibrate"
	  ret2=strncmp(input,"angle ",6);								//ret2 stores return value of string comparision with "angle"
	  ret3=strncmp(input,"pitch ",6);								//ret3 stores return value of string comparision with "pitch"

	  if(ret1==0)													//if string comparison returns 0(r1=0)
	  {
		  printf("\n\rBegin Calibration.....\n\r");					//begin recalibration
		  calibrate();												//call calibrate function
		  flag++;													//flag incremented so user is provided menu again
		  input_angle=0;											//input angle set to 0
		  input_pitch=0;											//input pitch set to 0
		  ret1=1;													//return value reset for new input
	  }

	  else if(ret2==0)												//if string comparison returns 0(r1=0)
	  {
		  strncpy(input_ang,input,6);								//copy the input to buf pointer
		  sscanf(input, "%s %f", input_ang, &input_angle);			//store input angle in the input_angle variable
		  if(input_angle==0|| input_angle>180|| input_angle<-180){	//if angle not in range
			  printf("\n\rPlease enter a valid angle!!\n\r");
			  flag++;												//increment flag to print menu again
			  input_angle=0;										//reinitialize input_angle for new input
		  }
		  else{
	      set_angle(input_angle);									//set the angle to that position
		  }
		  input_pitch=0;											//set input pitch as zero so change in pitch not calculated
		  ret2=1;													//return value reset for new input
	  }

	  else if(ret3==0)
	  {
		  strncpy(input_pit,input,6);								//copy the input to buf pointer
		  sscanf(input, "%s %f", input_pit, &input_pitch);			//store input pitch in the input_pitch variable
		  if(input_pitch==0|| input_pitch>90|| input_pitch<-90){	//if pitch not in range
			  printf("\n\rPlease enter a valid pitch!!\n\r");
			  flag++;												//increment flag to print menu again
			  input_pitch=0;										//reinitialize input_pitch for new input
		  }
		  else{
	      set_pitch(input_pitch);									//set the pitch to that position
		  }
		  input_angle=0;											//set input angle as zero so change in angle not calculated
		  ret3=1;													//return value reset for new input
	  }

	  else
	  {
		  printf("\n\rUnknown command: %s\n\r",input);		     				//unknown command received
	      printf("\n\rPlease enter from one of the menu options\n\r");		    //unknown command received
	      flag++;																//flag incremented so user is provided menu again
	      input_angle=0;														//set input angle as zero so change in angle not calculated
	      input_pitch=0;														//set input pitch as zero so change in pitch not calculated
	  }

}

//check menu_and_command.h for brief
void menu_and_input()
{
	  int i=0, wspace_flag=0;										//i is the index for input buffer
	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	//wspace_flag is a flag to count the whitespaces in the command


	  printf("\n\rEnter options from below menu:\n\r");
	  printf("1) Enter calibrate for re-calibration \n\r");
	  printf("2) Enter 'angle' and value in angle <value> format\n\r"
			  "for setting the gauge to specific angle(between -180 to +180)\n\r");
	  printf("3)If the reference is set to any flat surface you can also set the pitch!!\n\r "
			  "Enter 'pitch' and value in pitch <value> format\n\r"
			  "for setting the gauge to specific pitch(between -90 to +90)\n\r");
	  printf(">>");

	  /*Below loop clears the input buffer every time for new input*/
	  for(int j=0;input[j]!='\0';j++)
	  {
		 input[j]='\0';
	  }
return_to_home: while((input[i]=getchar())!='\r')				//taking input until user presses enter
			 	{
//Condition to remove heading space
					if(input[0]==' ')							//checking for heading whitespace
					{
						input[0]='\0';							//if whitespace recieved set buffer first position back to null
						goto return_to_home;					//return to take input
					}
					if(input[i]==CHK_BCKSP)						//checking if input buffer has a backspace

					{
						wspace_flag=2;							//setting the whitespace flag to 2
					}
//Condition to remove trailing space
					if(input[i]==' ' && wspace_flag==1)			//checking if more than one whitespace recieved and the flag is set to 1
					{
						input[i]='\0';						    //just considers the first whitespace and nullifies buffer for later whitespaces
						goto return_to_home;					//return to take input
					}
					putchar(input[i]);							//echoing the character back to the terminal

					if(wspace_flag==2)							//checking if the whitespace flag is 2(backspace received)
					{
						wspace_flag=0;							//if so the flag is reset
						input[i]='\0';							//pass null to buffer
						i--;									//decrement the index
						goto return_to_home;					//return to take input
					}
//Converting all characters to lower case for comparision
					if(input[i]>=65 && input[i]<=90)
						input[i]+=32;
					i++;										//incrementing the index for next character
//Condition to check trailing space after "calibrate"
					if(!(strcmp(input,"calibrate")))
					{
						wspace_flag=1;
						goto return_to_home;
					}
	     		}
	  wspace_flag=0;															//Resetting whitespace flag for another input
	  input[i]='\0';															//appending null at the end
	  command_processor();														//call the command processor to process the input
	  i=0;																		//resetting index to 0
}

//check menu_and_command.h for brief
void check()
{
	float angle_check=0.0;					//variables to check change in angle
	 if(input_angle!=0)										//if user enters input angle
	 {
	        read_full_xyz();								//read full xyz
	        convert_xyz_to_roll_pitch();					//convert the x y and z into roll and pitch

	        angle_check=roll-roll_cal;						//angle check checks for current positon wrt roll of accelerometer

	        if(angle_check<-180)							//angle check is set according to reference to check change from reference position
		    {
		    	angle_check+=360;
		    }

		    if(angle_check>180)								//angle check is set according to reference to check change from reference position
		    {
		    	angle_check-=360;
		    }

		    if(fabs(fabs(angle_check)-fabs(input_angle)) >= 10)		    //if the change in angle is greater than 10 degrees
	      	{
		    	angle_check=0;								//reset angle check
	      	    printf("ANGLE CHANGED\n\r");				//inform the user on the terminal that angle is changed
	      		blink(0,255,0);								//blinking the led for informing the user
	      		menu_and_input();							//call the menu again
	      	}
	 }
	 if(input_pitch!=0)
	 {
	        read_full_xyz();
	        convert_xyz_to_roll_pitch();

	        if(fabs(fabs(pitch)-fabs(input_pitch))>=10)			//if the change in pitch is greater than 10 degrees
	      	{
	      		pitch=0;								//reset pitch check
	      	    printf("PITCH CHANGED\n\r");				//inform the user on the terminal that pitch is changed
	      		blink(0,0,255);								//blinking the led for informing the user(blue for pitch)
	      		menu_and_input();							//call the menu again
	      	}
	 }
	 else if(flag>0)										//check if user has recalibrated or entered invalid input
	 {
		    flag=0;											//reset the flag
		    menu_and_input();								//call menu again
	 }
}

