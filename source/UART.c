/*
* *************************************************************************************************
 * UART.c
 * brief: The current c source file provides implementation of system calls syswrite and sysreadc to
 * 		  glue printf, sscanf and getchar to my UART.
 * 		  The code also contains initialization of UART registers and flags and UART interrupt handler.
 * 		  The UART initialization function also initializes the transmit and receive buffers
 * @Author: Michelle Christian
 * *************************************************************************************************
 */

/*****************************************INCLUDES**************************************************/
#include "UART.h"
#include "cbfifo.h"
#include "stddef.h"

/******************************************MACROS**************************************************/
#define DATA_BITS 0							//data bits for UART 0 value in register defines 8 bits
#define STOP_BITS 0							//stop bits 1
#define PARITY_BITS 0						//parity bits 0
#define UART_OVERSAMPLE_RATE (16)			//oversample rate 16
#define SYSCLOCK_FREQUENCY (24000000U)		//sysclock used for UART 24MHz

/***************************************FUNCTION DEFINITIONS****************************************/
//check implementation brief in uart.h
int __sys_write(int handle, char *buf, int size)
{
	  if(handle && buf!=NULL){
	  			  //copy characters up to null terminator
	  			  while(Tx_is_Full());			//wait for space to open up
	  			  Tx_cbfifo_enqueue(buf,size);

	  		  //start transmitter if it isn't already running
	  		  if(!(UART0->C2 & UART0_C2_TIE_MASK))
	  		  {
	  			  UART0->C2 |= UART0_C2_TIE(1);
	  		  }
	  		  return 0;
	  }
	  else{
		  return -1;
	  }
}

//check implementation brief in uart.h
int __sys_readc(void)
{
	  char c;
	  while(Rx_cbfifo_length()==0); 		//wait for character to arrive
	  if(Rx_cbfifo_dequeue(&c,1) != 1)		//dequeue on character/data
	  {
		return -1;							//if error return -1
	  }
	  else
	  {
		return c;							//else return the data
	  }
}

//check implementation brief in uart.h
void Init_UART0(uint32_t baud_rate)
{
	uint16_t sbr;

	//Enable clock gating for UART0 and Port A
	SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;

	//Make sure transmitter and receiver are disabled before init
	UART0->C2 &= ~UART0_C2_TE_MASK & ~UART0_C2_RE_MASK;

	//Configure UART0 Source clock
	SIM->SOPT2 |= SIM_SOPT2_UART0SRC(1);

	//Set pins to UART0 Rx and Tx
	PORTA->PCR[1] = PORT_PCR_ISF_MASK | PORT_PCR_MUX(2); //RX
	PORTA->PCR[2] = PORT_PCR_ISF_MASK | PORT_PCR_MUX(2); //RX

	//Set baud rate and oversampling ratio
	sbr= (uint16_t)((SYSCLOCK_FREQUENCY)/(baud_rate * UART_OVERSAMPLE_RATE));
	UART0->BDH &= ~UART0_BDH_SBR_MASK;
	UART0->BDH |= UART0_BDH_SBR(sbr>>8);
	UART0->BDL |= UART0_BDL_SBR(sbr);
	UART0->C4 |= UART0_C4_OSR(UART_OVERSAMPLE_RATE-1);

	//Enable interrupt for RX active edge and LIN break detect, select two stop bits
	UART0->BDH |= UART0_BDH_RXEDGIE(0) | UART0_BDH_SBNS(STOP_BITS) | UART0_BDH_LBKDIE(0);

	//Don't enable loopback mode, use 8 data bit mode, don't use parity
	UART0->C1 = UART0_C1_LOOPS(0) | UART0_C1_M(DATA_BITS) | UART0_C1_PE(PARITY_BITS);

	//Don't invert transmit data, do enable interrupt for errors
	UART0->C3 = UART0_C3_TXINV(0) | UART0_C3_ORIE(1) | UART0_C3_NEIE(1) | UART0_C3_FEIE(1) | UART0_C3_PEIE(1);

	//Clear error flags
	UART0->S1 = UART0_S1_OR(1) | UART0_S1_NF(1) |UART0_S1_FE(1) | UART0_S1_PF(1);

	//Send LSB first and do not invert received data
	UART0->S2 = UART0_S2_MSBF(0) | UART0_S2_RXINV(0);

	//Enable UART transmitter and receiver
	UART0->C2 |= UART0_C2_TE(1) | UART0_C2_RE(1) | UART_C2_RIE(1);

    NVIC_SetPriority(UART0_IRQn,2); 								//UART interrupt priority set to 2
    NVIC_ClearPendingIRQ(UART0_IRQn);								//Cleared pending IRQ
    NVIC_EnableIRQ(UART0_IRQn);										//Enable UARTinterrupt

	  Tx_Buff_Init();
	  Rx_Buff_Init();

}

//check implementation brief in uart.h
void UART0_IRQHandler(void)
{
	uint8_t ch;

	if(UART0->S1 & (UART_S1_OR_MASK | UART_S1_NF_MASK | UART_S1_FE_MASK | UART_S1_PF_MASK ))
	{
		//clear the error flags
		UART0->S1 |= UART0_S1_OR_MASK | UART0_S1_NF_MASK | UART0_S1_FE_MASK | UART0_S1_PF_MASK;
		//read the data register to clear RDRF
		ch=UART0->D;
	}

	if(UART0->S1 & UART0_S1_RDRF_MASK)
	{
		//received a character
		ch=UART0->D;
		if(!(Rx_is_Full()))
		{
			Rx_cbfifo_enqueue(&ch,1);
		}else{
			//error - queue full
			//discard character
		}
	}
	if((UART0->C2 & UART0_C2_TIE_MASK) &&	//transmitter interrupted
	   (UART0->S1 & UART0_S1_TDRE_MASK)){    //tx buffer empty
		//can send another character
		if(!(Tx_is_Empty()))
		{
			Tx_cbfifo_dequeue(&ch,1);
			UART0->D = ch;
		}else{
			//queue is empty so disable transmitter interrupt
			UART0->C2 &= ~UART0_C2_TIE_MASK;
		}
	}
}
