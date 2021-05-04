/*
 * **********************************************************************************************
 * cbfifo_tester.h
 *	This file contains prototype for the testing function used for checking cbfifo implementation in UART
 *  Created on: May 3, 2021
 *      Author: Howdy Pierce
 * **********************************************************************************************
 */

#ifndef CBFIFO_TESTER_H_
#define CBFIFO_TESTER_H_

//tests the cbfifo.
//Here only recieve fifo is tested but one can also check transmit fifo as the behaviour is same
void test_Rx_cbfifo();

#endif /* CBFIFO_TESTER_H_ */
