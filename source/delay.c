/*
 * **********************************************************************************************
 * delay.c
 * brief: Contains function to provide delay in milliseconds
 *      Author: Michelle Christian
 * **********************************************************************************************
 */

#include <MKL25Z4.H>

/***************************************FUNCTION DEFINITIONS****************************************/
void Delay (uint32_t dly) {
  volatile uint32_t t;

	for (t=dly*10000; t>0; t--)
		;
}
