/*
 * *************************************************************************************************
 * TPM.h
 *  This file contains functions that initialize PWM and provides specific colors values for LED
 *      Author: Michelle Christian
 * *************************************************************************************************
 */

#ifndef TPM_H_
#define TPM_H_

#include "MKL25Z4.h"

/*function		: 	TPM_init
 *param			: 	none
 *brief			:  	initializes the PWM
 *			  		MCG Multipurpose clock generator C1,C2 control registers
 * 			  		IREFS internal reference select
 *			 		IRCS internal reference clock select
 *					SOPT2 -Systems Options register 2
 *			  		TPMSRC Timer/PWM source select
 *  		  		SCGC clock gating register
 *return_type	: 	void
 */
void TPM_init();

/*function		:   color
 *param			:   three 8 bit unsigned integers for color change
 *brief			: 	gives value out to TPM register to provide different color ouputs on RGB LED
 *return_type	:   void
 */
void color(uint8_t r1,uint8_t g1, uint8_t b1);

#endif /* TPM_H_ */
