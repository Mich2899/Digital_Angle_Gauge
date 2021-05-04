/*
 * **********************************************************************************************
 * cbfifo.h
 * brief: The current file explains different buffer functions such as
 * 	      isEmpty, is Full, cbfifo_length, cbfifo_capacity, cbfifo_enqueue and cbfifo_dequeue
 * 	      This procedure is followed for both RX and TX
 *      Author: Michelle Christian
 * **********************************************************************************************
 */
#ifndef CBFIFO_H_
#define CBFIFO_H_

/*
 * function		:	 	 Tx_Buff_Init
 * param		:		 none
 * brief		:		 initializes the transmit buffer parameters
 * return_type	:	  	 void
 */
void Tx_Buff_Init();

/*
 * function		:	 	 Buff_Init
 * param		:		 none
 * brief		:		 initializes the receive buffer parameters
 * return_type	:	  	 void
 */
void Rx_Buff_Init();

/*
 * function		:	 	 Tx_is_Empty
 * param		:		 none
 * brief		:		 checks if the transmit buffer is empty
 * return_type	:	  	 int
 */
int Tx_is_Empty();

/*
 * function		:	 	 Rx_is_Empty
 * param		:		 none
 * brief		:		 checks if the receive buffer is empty
 * return_type	:	  	 int
 */
int Rx_is_Empty();

/*
 * function		:	 	 Tx_is_Full
 * param		:		 none
 * brief		:		 checks if the transmit buffer is full
 * return_type	:	  	 int
 */
int Tx_is_Full();

/*
 * function		:	 	 Rx_is_Full
 * param		:		 none
 * brief		:		 checks if the receive buffer is full
 * return_type	:	  	 int
 */
int Rx_is_Full();

/*
 * function		:	 	 Rx_cbfifo_enqueue
 * param		:		 buf      Pointer to the data
 *   				     nbyte    Max number of bytes to enqueue
 * brief		:		 Enqueues data onto the Rx FIFO, up to the limit of the available FIFO
 * 						 capacity.The number of bytes actually enqueued, which could be 0. In case
 * 						 of an error, returns -1.
 * return_type	:		 size_t
 */
size_t Rx_cbfifo_enqueue(void *buf, size_t nbyte);

/*
 * function		:	 	 Tx_cbfifo_enqueue
 * param		:		 buf      Pointer to the data
 *   				     nbyte    Max number of bytes to enqueue
 * brief		:		 Enqueues data onto the Tx FIFO, up to the limit of the available FIFO
 * 						 capacity.The number of bytes actually enqueued, which could be 0. In case
 * 						 of an error, returns -1.
 * return_type	:		 size_t
 */
size_t Tx_cbfifo_enqueue(void *buf, size_t nbyte);

/*
 * function		:	 	 Rx_cbfifo_dequeue
 * param		:		 *buf     Destination for the dequeued data
 *					     nbyte    Bytes of data requested
 *
 * brief		:        Attempts to remove ("dequeue") up to nbyte bytes of data from the Rx
 * 						 FIFO. Removed data will be copied into the buffer pointed to by buf.
 * 						 The number of bytes actually copied, which will be between 0 and
 * 						 nbyte.To further explain the behavior: If the FIFO's current length is 24
 * 						 bytes, and the caller requests 30 bytes, cbfifo_dequeue should
 * 						 return the 24 bytes it has, and the new FIFO length will be 0. If
 * 						 the FIFO is empty (current length is 0 bytes), a request to dequeue
 * 						 any number of bytes will result in a return of 0 from
 * 						 cbfifo_dequeue.
 * return_type	:		 size_t
 */
size_t Rx_cbfifo_dequeue(void *buf, size_t nbyte);

/*
 * function		:	 	 Tx_cbfifo_dequeue
 * param		:		 *buf     Destination for the dequeued data
 *					     nbyte    Bytes of data requested
 *
 * brief		:        Attempts to remove ("dequeue") up to nbyte bytes of data from the Tx
 * 						 FIFO. Removed data will be copied into the buffer pointed to by buf.
 * 						 The number of bytes actually copied, which will be between 0 and
 * 						 nbyte.To further explain the behavior: If the FIFO's current length is 24
 * 						 bytes, and the caller requests 30 bytes, cbfifo_dequeue should
 * 						 return the 24 bytes it has, and the new FIFO length will be 0. If
 * 						 the FIFO is empty (current length is 0 bytes), a request to dequeue
 * 						 any number of bytes will result in a return of 0 from
 * 						 cbfifo_dequeue.
 * return_type	:		 size_t
 */
size_t Tx_cbfifo_dequeue(void *buf, size_t nbyte);

/*
 * function		:	 	 Rx_cbfifo_length
 * param		:		 none
 * brief		:		 Returns the number of bytes currently on the Rx FIFO.
 * return_type	:		 int
 */
int Rx_cbfifo_length();

/*
 * function		:	 	 Tx_cbfifo_length
 * param		:		 none
 * brief		:		 Returns the number of bytes currently on the Tx FIFO.
 * return_type	:		 int
 */
int Tx_cbfifo_length();

/*
 * function		:	 	 Rx_cbfifo_capacity
 * param		:		 none
 * brief		:		 Returns the capacity in bytes of the Rx FIFO
 * return_type	:		 size_t
 */
size_t Rx_cbfifo_capacity();

/*
 * function		:	 	 Tx_cbfifo_capacity
 * param		:		 none
 * brief		:		 Returns the capacity in bytes of the Tx FIFO
 * return_type	:		 size_t
 */
size_t Tx_cbfifo_capacity();


#endif /* CBFIFO_H_ */

