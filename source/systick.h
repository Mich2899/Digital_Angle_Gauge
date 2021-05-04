/*
 * *************************************************************************************************
 * systick.h
 *  This file contains explanation for initialization and functions for the systick timer
 *     @Author: Michelle Christian
 * *************************************************************************************************
 */

#ifndef SYSTICK_H_
#define SYSTICK_H_

#include <stdint.h>

typedef uint32_t ticktime_t; // time since boot in twentieth of a second

extern ticktime_t boot_time;

/*function:     init_systick
 *parameters:   none
 *explanation: 	initializes the systick timer with 2.4MHz frequency
 *return type:  void
 */
void init_systick();

/*function:     reset_timer
 *parameters:   none
 *explanation: 	resets the boot_time variable to return value every time the timer is reset
 *return type:  void
 */
void reset_timer();

/*function:     get_timer
 *parameters:   none
 *explanation: 	returns the boot_time variable (time after the timer is reset)
 *return type:  ticktime_t
 */
ticktime_t get_timer();

#endif /* SYSTICK_H_ */
