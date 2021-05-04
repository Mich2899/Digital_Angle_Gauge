/*
 * UART.h
 *	This file contains explanation for functions used by UART
 *	syswrite to use printf
 *	sysread to use sscanf
 *	UART0 initialization and UART0 handler
 *     @Author: Michelle Christian
 */

#ifndef UART_H_
#define UART_H_

#include <stdint.h>
#include "MKL25Z4.h"
#include "sysclock.h"

/*
 * function		:	 	 __sys_write
 * param		:		 int handle		handle=1 for stdout
 * 						 char* buf 		buf is a character pointer where we pass the string to be printed
 * 				 		 int size		size is the length of the string passed in buf
 * brief		:		 system function is re-written to glue UART with printf
 * return_type	:	  	 int
 */
int __sys_write(int handle, char *buf, int size);

/*
 * function		:	 	 __sys_readc
 * param		:		 void
 * brief		:		 system function is re-written to glue UART with getchar and sscanf
 * return_type	:	  	 int
 */
int __sys_readc(void);



/*
 * function		:	 	 Init_UART0
 * param		:		 uint32_t baud_rate
 * brief		:		 initializes the clock gating registers,
 * 					     enables the transmitter and receiver GPIO pins
 * 					     initializes UART0 to the provided baud rate
 * return_type	:	  	 void
 */
void Init_UART0(uint32_t baud_rate);

/*
 * function		:	 	 UART0_IRQHandler
 * param		:		 void
 * brief		:		 Interrupt service routine for UART0
 * return_type	:	  	 void
 */
void UART0_IRQHandler(void);

#endif /* UART_H_ */
