/*
 * **********************************************************************************************
 * @file    i2c.c
 * @brief   This file contains functions for i2c including
 * 			initialization, busy condition check, wait for transmission/reception to complete,
 * 			i2c start, i2c read setup, i2c repeated read, i2c read byte, i2c write byte
 * @author  Michelle Christian
 * **********************************************************************************************
 */


/****************************************INCLUDES*************************************************/
#include <MKL25Z4.H>
#include "i2c.h"

/****************************************MACROS**************************************************/
#define I2C_M_START 	I2C0->C1 |= I2C_C1_MST_MASK		//When the MST bit is changed from a 0 to a 1,
														//a START signal is generated on the bus
														//and master mode is selected.

#define I2C_M_STOP  	I2C0->C1 &= ~I2C_C1_MST_MASK	//When this bit changes from a 1 to a 0,
														//a STOP signal is generated and the mode of operation
														//changes from master to slave.

#define I2C_M_RSTART 	I2C0->C1 |= I2C_C1_RSTA_MASK	//Writing a one to this bit generates a
														//repeated START condition provided
														//it is the current master

#define I2C_TRAN			I2C0->C1 |= I2C_C1_TX_MASK	//if bit is 1 Transmit
#define I2C_REC				I2C0->C1 &= ~I2C_C1_TX_MASK	//if bit is 0 Receive

#define BUSY_ACK 	    while(I2C0->S & 0x01)			//checks for acknowledgement
#define TRANS_COMP		while(!(I2C0->S & 0x80))		//checks for the transfer complete flag
#define I2C_WAIT 			i2c_wait();					//macro for i2c_wait function

#define NACK 	        I2C0->C1 |= I2C_C1_TXAK_MASK	//bit to check NACK
#define ACK           I2C0->C1 &= ~I2C_C1_TXAK_MASK		//bit to check ACK

int lock_detect=0;
int i2c_lock=0;

//**************************************FUNCTION DEFINITIONS***************************************
//check i2c.h for brief
void i2c_init(void)
{
	//clock i2c peripheral and port E
	SIM->SCGC4 |= SIM_SCGC4_I2C0_MASK;
	SIM->SCGC5 |= (SIM_SCGC5_PORTE_MASK);

	//set pins to I2C function
	PORTE->PCR[24] |= PORT_PCR_MUX(5);
	PORTE->PCR[25] |= PORT_PCR_MUX(5);

	//set to 100k baud
	//baud = bus freq/(scl_div+mul)
 	//~400k = 24M/(64); icr=0x12 sets scl_div to 64

 	I2C0->F = (I2C_F_ICR(0x10) | I2C_F_MULT(0));

	//enable i2c and set to master mode
	I2C0->C1 |= (I2C_C1_IICEN_MASK);

	// Select high drive mode
	I2C0->C2 |= (I2C_C2_HDRS_MASK);
}

//check i2c.h for brief
void i2c_busy(void){
	// Start Signal
	lock_detect=0;
	I2C0->C1 &= ~I2C_C1_IICEN_MASK;
	//transmission
	I2C_TRAN;
	//start bit
	I2C_M_START;
	//i2c enabled
	I2C0->C1 |=  I2C_C1_IICEN_MASK;
	// Write to clear line
	I2C0->C1 |= I2C_C1_MST_MASK; 			// set MASTER mode
	I2C0->C1 |= I2C_C1_TX_MASK; 			// Set transmit (TX) mode
	I2C0->D = 0xFF;
	while ((I2C0->S & I2C_S_IICIF_MASK) == 0U) {
	} // wait interrupt

	I2C0->S |= I2C_S_IICIF_MASK; 			// clear interrupt bit

	I2C0->S |= I2C_S_ARBL_MASK; 			// Clear arbitration error flag

	I2C0->C1 &= ~I2C_C1_IICEN_MASK;			// Send start
	I2C0->C1 |= I2C_C1_TX_MASK; 			// Set transmit (TX) mode
	I2C0->C1 |= I2C_C1_MST_MASK; 			// START signal generated

	I2C0->C1 |= I2C_C1_IICEN_MASK;			//Wait until start is send

	I2C0->C1 &= ~I2C_C1_IICEN_MASK;			// Send stop
	I2C0->C1 |= I2C_C1_MST_MASK;
	I2C0->C1 &= ~I2C_C1_MST_MASK; 			// set SLAVE mode
	I2C0->C1 &= ~I2C_C1_TX_MASK; 			// Set Rx
	I2C0->C1 |= I2C_C1_IICEN_MASK;

	// wait
	// Clear arbitration error & interrupt flag
	I2C0->S |= I2C_S_IICIF_MASK;
	I2C0->S |= I2C_S_ARBL_MASK;
	lock_detect=0;
	i2c_lock=1;
}

void i2c_wait(void) {
	//lock detect variable
	lock_detect = 0;
	//check forinterrupt flag and lock detect variable is less than 200
	while(((I2C0->S & I2C_S_IICIF_MASK)==0) & (lock_detect < 200)) {
		lock_detect++;
	}
	//as soon as greater than 200 check for busy condition
	if (lock_detect >= 200)
		i2c_busy();
	//enable the interrupt again
	I2C0->S |= I2C_S_IICIF_MASK;
}

//check i2c.h for brief
void i2c_start()
{
	I2C_TRAN;								//set to transmit mode
	I2C_M_START;							//send start
}

//check i2c.h for brief
void i2c_read_setup(uint8_t dev, uint8_t address)
{
	I2C0->D = dev;			  				//send dev address
	I2C_WAIT								//wait for completion

	I2C0->D =  address;						//send read address
	I2C_WAIT								//wait for completion

	I2C_M_RSTART;				  		    //repeated start
	I2C0->D = (dev|0x1);	 				//send dev address (read)
	I2C_WAIT							 	//wait for completion

	I2C_REC;						   		//set to receive mode

}

//check i2c.h for brief
uint8_t i2c_repeated_read(uint8_t isLastRead)
{
	uint8_t data;							//variable for dummy read

	lock_detect = 0;						//lock detect variable reinitialized

	if(isLastRead)	{
		NACK;								//set NACK after read
	} else	{
		ACK;								//ACK after read
	}

	data = I2C0->D;							//dummy read
	I2C_WAIT								//wait for completion

	if(isLastRead)	{
		I2C_M_STOP;							//send stop
	}
	data = I2C0->D;							//read data

	return  data;
}

//check i2c.h for brief
uint8_t i2c_read_byte(uint8_t dev, uint8_t address)
{
	uint8_t data;							//variable for dummy read

	I2C_TRAN;								//set to transmit mode
	I2C_M_START;							//send start
	I2C0->D = dev;			  				//send dev address
	I2C_WAIT								//wait for completion

	I2C0->D =  address;						//send read address
	I2C_WAIT								//wait for completion

	I2C_M_RSTART;				   			//repeated start
	I2C0->D = (dev|0x1);	 				//send dev address (read)
	I2C_WAIT							 	//wait for completion

	I2C_REC;						   		//set to recieve mode
	NACK;									//set NACK after read

	data = I2C0->D;							//dummy read
	I2C_WAIT								//wait for completion

	I2C_M_STOP;								//send stop
	data = I2C0->D;							//read data

	return data;
}



//check i2c.h for brief
void i2c_write_byte(uint8_t dev, uint8_t address, uint8_t data)
{

	I2C_TRAN;								//set to transmit mode
	I2C_M_START;							//send start
	I2C0->D = dev;			  				//send dev address
	I2C_WAIT						  		//wait for ack

	I2C0->D =  address;						//send write address
	I2C_WAIT

	I2C0->D = data;							//send data
	I2C_WAIT
	I2C_M_STOP;

}
