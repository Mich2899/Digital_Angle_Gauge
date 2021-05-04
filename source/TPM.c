/*
* *************************************************************************************************
 * TPM.c
 *  This file contains function that initializes PWM and sets the led to a specific color
 *      @Author: Michelle Christian
* *************************************************************************************************
 */

/*****************************************INCLUDES**************************************************/
#include "TPM.h"

/***************************************FUNCTION DEFINITIONS****************************************/
void TPM_init(){
    MCG->C1 |= MCG_C1_IREFS_MASK | MCG_C1_IRCLKEN_MASK;             //IREFS and IRCLKEN enabled in C1 register of MCG
    MCG->C2 |= MCG_C2_IRCS_MASK;                                    //IRCS set to 1 hence selects fast internal reference clock
    SIM->SCGC6 |=SIM_SCGC6_TPM0_MASK;                               //TPM0 enabled
    SIM->SCGC6 |= SIM_SCGC6_TPM2_MASK;                              //TPM2 enabled
    SIM->SOPT2 |= SIM_SOPT2_TPMSRC(3);                              //TPMSRC is set to 3 i.e. MCGIRCLK clock selected for TPM

    TPM2->SC = TPM_SC_CMOD(1) | TPM_SC_PS(4);                       //LPTPM clock selected for TPM2 with prescalar 16
    TPM2->MOD = 2500;                                               //mod register set to 2500

    TPM0->SC = TPM_SC_CMOD(1) | TPM_SC_PS(4);                       //LPTPM clock selected for TPM0 with prescalar 16
    TPM0->MOD = 2500;                                               //mod register set to 2500

    SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;                             //clock gating for LED pin
    SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;
    PORTB->PCR[18] = PORTB->PCR[19] = PORT_PCR_MUX(3);          	//multiplex value as 3 for TPM2                                                        //multiplex value as 3 for TPM2
    PORTD->PCR[1] = PORT_PCR_MUX(4);                   			  	//multiplex value as 4 for TPM0

    TPM2->CONTROLS[0].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK;              //rising edge set for TPM2 channel 0
    TPM2->CONTROLS[1].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK;              //rising edge set for TPM2 channel 1
    TPM0->CONTROLS[1].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK;              //rising edge set for TPM0 channel 1
}

void color(uint8_t r1,uint8_t g1, uint8_t b1)
{

		       TPM2->CONTROLS[0].CnV  = (r1*TPM2->MOD)/255;	//steady color R
    	       TPM2->CONTROLS[1].CnV  = (g1*TPM2->MOD)/255;	//steady color G
               TPM0->CONTROLS[1].CnV  = (b1*TPM0->MOD)/255;	//steady color B
}
