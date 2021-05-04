/*
 * *************************************************************************************************
 * cbfifo.c
 * brief: The current file initializes buffers and provides implementation of different buffer functions such as
 * 	      isEmpty, is Full, cbfifo_length, cbfifo_capacity, cbfifo_enqueue and cbfifo_dequeue
 * 	      cbfifo_enqueue puts data into the buffer in fifo manner
 * 	      cbfifo_dequeue removes data from the buffer in fifo manner
 * 	      This procedure is followed for both RX and TX
 *      Author: Michelle Christian
 * *************************************************************************************************
 */


/*****************************************INCLUDES**************************************************/
#include<stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "MKL25Z4.h"
#include "cbfifo.h"



/******************************************MACROS**************************************************/
#define SIZE 1024			//maximum buffer size



/***************************************GLOBAL VARIABLES******************************************/
/*
 * Structure that contains buffer parameters including
 * buffer array, head index, tail index and length that defines number of contents in the buffer */
typedef struct cbfifo{
	uint8_t buffer[SIZE];
	unsigned int head;		//Index of oldest data element
	unsigned int tail;		//Index of next free space
	unsigned int length;	//Number of elements in use
	unsigned int capacity;
}c_buff;

c_buff TxQ;					//cbfifo variable for transmitter
c_buff RxQ;					//cbfifo variable for receiver



/***************************************FUNCTION DEFINITIONS****************************************/
void Rx_Buff_Init()
{
	unsigned int i;
	for(i=0; i<SIZE; i++)
		RxQ.buffer[i]=0;							//clearing the buffer
	RxQ.head=0;									//initializing index of oldest data element to 0
	RxQ.tail=0;									//initializing index of next free space to 0
	RxQ.length=0;								//initializing number of elements in use to 0
	RxQ.capacity=SIZE;							//defining the maximum capacity of the fifo
}


//check function brief in cbfifo.h
void Tx_Buff_Init()
{
	unsigned int i;
	for(i=0; i<SIZE; i++)
		TxQ.buffer[i]=0;							//clearing the buffer
	TxQ.head=0;									//initializing index of oldest data element to 0
	TxQ.tail=0;									//initializing index of next free space to 0
	TxQ.length=0;								//initializing number of elements in use to 0
	TxQ.capacity=SIZE;							//defining the maximum capacity of the fifo
}


//below function checks if the fifo for Rx is empty
//check function brief in cbfifo.h
int Rx_is_Empty()
{
	return RxQ.length ==0;						//returning 1 if the length of the buffer is 0 else returns 0
}


//below function checks if the fifo for Tx is empty
//check function brief in cbfifo.h
int Tx_is_Empty()
{
	return TxQ.length ==0;						//returning 1 if the length of the buffer is 0 else returns 0
}


//check function brief in cbfifo.h
int Rx_is_Full()
{
	return RxQ.length == SIZE;					//returning 1 if the length of the buffer is SIZE else returns 0
}


//check function brief in cbfifo.h
int Tx_is_Full()
{
	return TxQ.length == SIZE;					//returning 1 if the length of the buffer is SIZE else returns 0
}


//check function brief in cbfifo.h
size_t Rx_cbfifo_enqueue(void *buf, size_t nbyte)
{
	uint32_t masking_state;

	size_t temp=0;								//temp variable gives the number of bytes we can enqueue on the fifo
	RxQ.length=Rx_cbfifo_length();				//updating the length of RxQ buffer

	if(Rx_is_Full()==1)
	{
		return 0; 	// if the buffer is full it will not enqueue any bytes
	}
		if(SIZE>(RxQ.length)+nbyte)
		{
			temp=nbyte;	//if the sum of length and nbytes is less than 128 then it will enqueue nbytes
		}
		else
		{
			temp=SIZE-(RxQ.length);// else it will enqueue only the remaining empty spaces.
		}
			for(int j=0;j<temp;j++)
			{
				RxQ.buffer[RxQ.tail++] =*(uint8_t*)buf ;
				RxQ.tail %= SIZE;
				buf++;  //incrementing buffer address
				//protect b.length++ operation from preemption
				//save current masking state
				masking_state= __get_PRIMASK();
				//disable interrupts
				__disable_irq();
				//update variable
				RxQ.length++;
				//restore interrupt masking state
				__set_PRIMASK(masking_state);
			}
	return temp;
}


//check function brief in cbfifo.h
size_t Tx_cbfifo_enqueue(void *buf, size_t nbyte)
{
	uint32_t masking_state;

	size_t temp=0;					//temp variable gives the number of bytes we can enqueue on the fifo
	TxQ.length=Tx_cbfifo_length();	//updating the length of TxQ buffer

	if(Tx_is_Full()==1)
	{
		return 0; 	// if the buffer is full it will not enqueue any bytes
	}
		if(SIZE>(TxQ.length)+nbyte)
		{
			temp=nbyte;	//if the sum of length and nbytes is less than 128 then it will enqueue nbytes
		}
		else
		{
			temp=SIZE-(TxQ.length);// else it will enqueue only the remaining empty spaces.
		}
			for(int j=0;j<temp;j++)
			{
				TxQ.buffer[TxQ.tail++] =*(uint8_t*)buf ;		//cast void *buf into type of TxQ buffer
				TxQ.tail %= SIZE;
				buf++;  //incrementing buffer address
				//protect b.length++ operation from preemption
				//save current masking state
				masking_state= __get_PRIMASK();
				//disable interrupts
				__disable_irq();
				//update variable
				TxQ.length++;
				//restore interrupt masking state
				__set_PRIMASK(masking_state);
			}
	return temp;
}


//check function brief in cbfifo.h
size_t Rx_cbfifo_dequeue(void *buf, size_t nbyte)
{
	uint32_t masking_state;

	size_t temp=0;
	RxQ.length=Rx_cbfifo_length();		//updating the length of RxQ buffer

	if(Rx_is_Empty()==1)
	{
		return 0; // if the buffer is empty it will not dequque any element
	}
		if(nbyte>(RxQ.length))
		{
			temp=RxQ.length; // if condition checks if there are enough bytes to enqueue if not it will enqueue remaining bytes
		}
		else
		{
			temp=nbyte;		//if enough number of elements present in the buffer it will dequeue all the nbytes
		}
			uint8_t *p;
			p=(uint8_t *)buf;		//casting the void buffer into type of RxQ.buffer (unsigned integer of 8 bits)
			int i=0;
			while(i<=temp-1){
			p[i]=RxQ.buffer[RxQ.head];		//dequeue buffer
			RxQ.buffer[RxQ.head++]='_';		//increment position
			RxQ.head %=SIZE;
			//protect b.length-- operation from preemption
			//save current masking state
			masking_state= __get_PRIMASK();
			//disable interrupts
			__disable_irq();
			//update variable
			RxQ.length--;
			//restore interrupt making state
			__set_PRIMASK(masking_state);
			i++;
		}
	return temp;
}


//check function brief in cbfifo.h
size_t Tx_cbfifo_dequeue(void *buf, size_t nbyte)
{
	uint32_t masking_state;

	size_t temp=0;
	TxQ.capacity=Tx_cbfifo_capacity();
	TxQ.length=Tx_cbfifo_length();	//updating the length of TxQ buffer

	if(Tx_is_Empty()==1)
	{
		return 0; // if the buffer is empty it will not dequque any element
	}
		if(nbyte>(TxQ.length))
		{
			temp=TxQ.length; // if condition checks if there are enough bytes to enqueue if not it will enqueue remaining bytes
		}
		else
		{
			temp=nbyte;		//if enough number of elements present in the buffer it will dequeue all the nbytes
		}
			uint8_t *p;
			p=(uint8_t *)buf;	//casting the void buffer into type of TxQ.buffer (unsigned integer of 8 bits)
			int i=0;
			while(i<=temp-1){
			p[i]=TxQ.buffer[TxQ.head];		//dequeue buffer
			TxQ.buffer[TxQ.head++]='_';		//increment position
			TxQ.head %=SIZE;
			//protect b.length-- operation from preemption
			//save current masking state
			masking_state= __get_PRIMASK();
			//disable interrupts
			__disable_irq();
			//update variable
			TxQ.length--;
			//restore interrupt making state
			__set_PRIMASK(masking_state);
			i++;
		}
	return temp;				//return the number of dequeued bytes
}


//check function brief in cbfifo.h
int Rx_cbfifo_length()
{
	return RxQ.length;							//returns length of Rx buffer
}


//check function brief in cbfifo.h
int Tx_cbfifo_length()
{
	return TxQ.length;							//returns length of Tx buffer
}


//check function brief in cbfifo.h
size_t Rx_cbfifo_capacity()
{
	return RxQ.capacity;						//returns capacity of Rx buffer
}


//check function brief in cbfifo.h
size_t Tx_cbfifo_capacity()
{
	return TxQ.capacity;						//returns capacity of Tx buffer
}
