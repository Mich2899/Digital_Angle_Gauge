/**
 *  * **********************************************************************************************
 * @file    FINAL_1.c
 * @brief   This source file initializes all the modules and calls the bubble level demo
 * 			asks the user to calibrate the capacitive touch according to a reference level
 * 			and provides a menu to the user. If the user enters to re-calibrate it,
 * 			they are prompted to touch the sensor again to set the reference again. If the
 * 			user enters a specific angle to set, they are prompted to rotate the board
 * 			and once the angle is met, green led glows to indicate the same. If the angle changes
 * 			from the set position the led starts blinking and the user is also informed using
 * 			the uart terminal. The user is provided the menu again at this point so as to set
 * 			a different angle or re-calibrate the digital gauge.
 * @author  Michelle Christian
 *  * **********************************************************************************************
 */

/*****************************************INCLUDES**************************************************/
#include <stdio.h>
#include <math.h>
#include <menu_and_command.h>
#include <set_gauge.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "MKL25Z4.h"
#include "UART.h"
#include "startup.h"
#include "cbfifo_tester.h"

int main(void) {

  	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();
#endif


	  init_all();										//initializes all the modules
	  printf("Running module tests\n\r");
      test_Rx_cbfifo();									//tests the cbfifo used for UART
	  test_TSI();										//test capcitive touch input
	  bubble_demo();									//Bubble_level demo demonstrated for 7 seconds
      printf("all tests complete\n\r");					//Informs the user tests are complete
	  printf("\n\rWelcome to Digital Angle Gauge!\n\r");	//Message to user that the module can be calibrated and used for any application
	  calibrate();										//initial calibration of the gauge
	  printf("\n\rAll set to use the gauge!!\n\r\n\r");		//once the angle is set the user is set to use the gauge
	  menu_and_input();									//user provided a menu

	  while(1)
	   {
		  check();										//checks if there is change in angle from the user set angle
	   }

    return 0 ;
}
