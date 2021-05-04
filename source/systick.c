/*
 * *************************************************************************************************
 * systick.c
 *		This file contains systick functionalities set at 24MHz clock frequency
 *	@Author: Michelle Christian
 * *************************************************************************************************
 */

/*****************************************INCLUDES**************************************************/
#include "systick.h"
#include <stdint.h>
#include "clock_config.h"
#include "MKL25Z4.h"
#include "sysclock.h"


/******************************************MACROS**************************************************/
#define CLOCK_FREQUENCY (24000000U)	//clock frequency set to 24MHz


/***************************************GLOBAL VARIABLES******************************************/
ticktime_t time=0;				//time after which systick interrupt handler is called
ticktime_t boot_time=0;			//time after systick timer is reset
int sys_time=0;


/***************************************FUNCTION DEFINITIONS****************************************/
void init_systick()
{
	SysTick->LOAD = (CLOCK_FREQUENCY/50)-1;		   		//load the systick timer register to get interrupt every 20 milliseconds
	SysTick->VAL  =	0;							   		//force load of reload value
	time		  =	((CLOCK_FREQUENCY/50)/24000);		//20 milliseconds time(time after which systick interrupt handler is called)
	NVIC_SetPriority(SysTick_IRQn,3);		   			//set interrupt priority
	SysTick->CTRL = SysTick_CTRL_TICKINT_Msk |			//Enabling the interrupt
					SysTick_CTRL_ENABLE_Msk  | 			//enabling the systick timer
					SysTick_CTRL_CLKSOURCE_Msk;			//using the internal clock source
}

void reset_timer()
{
	boot_time=0;										//setting the variable zero every time timer is reset
}

ticktime_t get_timer()
{
	return boot_time;									//returns the current value of timer
}

void SysTick_Handler()
{
	boot_time += time;									//time since last reset incremented every 20ms
}

