/**
 * **********************************************************************************************
 * @file    i2c.h
 * @brief   This file explains functions for i2c including
 * 			initialization, busy condition check, wait for transmission/reception to complete,
 * 			i2c start, i2c read setup, i2c repeated read, i2c read byte, i2c write byte
 * @author  Michelle Christian
 * **********************************************************************************************
 */

#ifndef I2C_H
#define I2C_H
#include <stdint.h>

/*
 * function		: i2c_init
 * param		: void/none
 * brief		: initializes the i2c parameters
 * return_type	: void
 */
void i2c_init(void);


/*
 * function		: i2c_start
 * param		: void/none
 * brief		: initializes i2c module into start mode and sents start bit
 * return_type	: void
 */
void i2c_start(void);


/*
 * function		: i2c_read_setup
 * param		: uint8_t dev			device address
 * 				  uint8_t address		register address
 * brief		: prepares the i2c module for read from the device by setting up the
 * 				  device address and register address
 * return_type	: void
 */
void i2c_read_setup(uint8_t dev, uint8_t address);

/*
 * function		: i2c_repeated_read
 * param		: uint8_t isLastRead
 * brief		: reads a byte and depending on the last read send ACK/NACK appropriately
 * 				  returns the data from dummy read
 * return_type	: uint8_t
 */
uint8_t i2c_repeated_read(uint8_t isLastRead);

/*
 * function		: i2c_read_byte
 * param		: uint8_t dev			device address
 * 				  uint8_t address		register address
 * brief		: uses 7 byte addressing and reads from the device
 * 				  returns the data from dummy read
 * return_type	: uint8_t
 */
uint8_t i2c_read_byte(uint8_t dev, uint8_t address);

/*
 * function		: i2c_write_byte
 * param		: uint8_t dev			device address
 * 				  uint8_t address		register address
 * 				  uint8_t data			data to be written
 * brief		: uses 7 byte addressing and writes to the device
 * return_type	: void
 */
void i2c_write_byte(uint8_t dev, uint8_t address, uint8_t data);

#endif
