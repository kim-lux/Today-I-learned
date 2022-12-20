/*
 * Copyright (c) 2014 - 2016, Freescale Semiconductor, Inc.
 * Copyright (c) 2016 - 2018, NXP.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY NXP "AS IS" AND ANY EXPRESSED OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL NXP OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "device_registers.h"	/* include peripheral declarations S32K144 */
#include "LPUART.h"

#define PTD0 0 // Port PTD, bit 0 : Output to blue LED
#define PTD15 15 // Port PTD, bit 0 : Output to red LED
#define PTD16 16 // Port PTD, bit 0 : Output to green LED
#define PTC12 12 // Port PTC, bit 12 : Input from SW2
#define PTC13 13 // Port PTC, bit 12 : Input from SW1

#define PTA0   0
#define PTA1   1
#define PTA2   2
#define PTA3   3


void PORT_init (void)
{
	/*!
	 * Pins definitions
	 * ===================================================
	 *
	 * Pin number        | Function
	 * ----------------- |------------------
	 * PTC6              | UART1 TX
	 * PTC7              | UART1 RX
	 */
    /*---------------------------UART SET---------------------------*/
    PCC->PCCn[PCC_PORTC_INDEX ]|=PCC_PCCn_CGC_MASK; /* Enable clock for PORTC */

    PORTC->PCR[6]|=PORT_PCR_MUX(2);	/* Port C6: MUX = ALT2, UART1 TX */
    PORTC->PCR[7]|=PORT_PCR_MUX(2);   /* Port C7: MUX = ALT2, UART1 RX */

	PCC-> PCCn[PCC_PORTC_INDEX] |= PCC_PCCn_CGC_MASK; // Enable clock to PORT C

	/*---------------------------SWITCH SET---------------------------*/
	PORTC ->PCR[12] &= ~PORT_PCR_MUX_MASK;
	PORTC ->PCR[13] &= ~PORT_PCR_MUX_MASK;
	PORTC ->PCR[12] |= PORT_PCR_MUX(1); // Port C12 MUX= GPIO
	PORTC -> PCR[13] |= PORT_PCR_MUX(1); // Port C13 MUX= GPIO
	PTC->PDDR &= ~(1<<PTC12); // Port C12: Data Direction : input (default)
	PTC->PDDR &= ~(1<<PTC13); // Port C12: Data Direction : input (default)
	PORTC ->PCR[12] |= 0x00090000; // Port C12 IRQC : Interrupt on ?-edge
	/*
	 	 * Interrupt on rising-edge : 0x00090000
	 	 * Interrupt on falling-edge : 0x000A0000
	 	 * Interrupt on rising & falling-edge : 0x000B0000
	*/

	/*--------------LED SET--------------*/
	PCC-> PCCn[PCC_PORTD_INDEX] |= PCC_PCCn_CGC_MASK; // Enable clock to PORT D

	PORTD ->PCR[0] &= ~PORT_PCR_MUX_MASK;
	PORTD ->PCR[15] &= ~PORT_PCR_MUX_MASK;
	PORTD ->PCR[16] &= ~PORT_PCR_MUX_MASK;
	PORTD ->PCR[0] |= PORT_PCR_MUX(1); // Port D0 MUX = GPIO
	PORTD ->PCR[15] |= PORT_PCR_MUX(1); // Port D15 MUX = GPIO
	PORTD ->PCR[16] |= PORT_PCR_MUX(1); // Port D16 MUX = GPIO

	PCC-> PCCn[PCC_PORTA_INDEX] = PCC_PCCn_CGC_MASK;/* Enable clocks to peripherals (PORT modules) */ /* Enable clock to PORT A*/
	PCC-> PCCn[PCC_PORTC_INDEX] = PCC_PCCn_CGC_MASK;/* Enable clocks to peripherals (PORT modules) */ /* Enable clock to PORT C*/

	PTA->PDDR |= 0x0F;       		 /* Port A0-A3: Data Direction= output */
	PORTA->PCR[0] = PORT_PCR_MUX(1); /* Port A0: MUX = GPIO */
	PORTA->PCR[1] = PORT_PCR_MUX(1); /* Port A1: MUX = GPIO */
	PORTA->PCR[2] = PORT_PCR_MUX(1); /* Port A2: MUX = GPIO */
	PORTA->PCR[3] = PORT_PCR_MUX(1); /* Port A3: MUX = GPIO */

	PTD->PDDR |= 1<<PTD0; // Port D0: Data Direction : output
	PTD->PDDR |= 1<<PTD15; // Port D15: Data Direction : output
	PTD->PDDR |= 1<<PTD16; // Port D16: Data Direction : output

	PTD-> PSOR |= 1<<PTD0; // Port D0: LED OFF
	PTD-> PSOR |= 1<<PTD15; // Port D15: LED OFF
	PTD-> PSOR |= 1<<PTD16; // Port D16: LED OFF

}

void WDOG_disable (void)
{
  WDOG->CNT=0xD928C520;     /* Unlock watchdog 		*/
  WDOG->TOVAL=0x0000FFFF;   /* Maximum timeout value 	*/
  WDOG->CS = 0x00002100;    /* Disable watchdog 		*/
}



void NVIC_init_IRQs(void)
{
   /* Timer Interrupt Init & Priority */
   S32_NVIC ->ICPR[1] |= 1 << (48 % 32); /* IRQ48 -LPIT0 ch0: Clear any pending IRQ */
   S32_NVIC ->ISER[1] |= 1 << (48 % 32); /* IRQ48 -LPIT0 ch0: Enable IRQ */
   S32_NVIC ->IP[48] = 0xB; /* IRQ48 -LPIT0 ch0: Priority 10 of 0-15, A=10 */
   /* Switch Interrupt Init & Priority */
   S32_NVIC ->ICPR[1] |= 1 << (61 % 32); // Clear any pending IRQ61
   S32_NVIC ->ISER[1] |= 1 << (61 % 32); // Enable IRQ61
   S32_NVIC ->IP[61] = 0xA; // Priority 11 of 0-15 B=11

}


void LPIT0_init (void)
{
    /*!
   * LPIT Clocking:
   * ==============================
   */
   PCC->PCCn[PCC_LPIT_INDEX] &= ~PCC_PCCn_PCS_MASK;
   PCC->PCCn[PCC_LPIT_INDEX] |= PCC_PCCn_PCS(6); /* Clock Src = 6 (SPLL2_DIV2_CLK)*/
   PCC->PCCn[PCC_LPIT_INDEX] |= PCC_PCCn_CGC_MASK; /* Enable clk to LPIT0 regs */
   /*!
   * LPIT Initialization:
   */
   LPIT0 ->MCR |= LPIT_MCR_M_CEN_MASK; /* DBG_EN -0: Timer chans stop in Debug mode */
   /* DOZE_EN=0: Timer chans are stopped in DOZE mode */
   /* SW_RST=0: SW reset does not reset timer chans , regs */
   /* M_CEN=1: enable module clk ( allows writing other LPIT0 regs) */
   LPIT0 ->MIER |= LPIT_MIER_TIE0_MASK; /* TIE0=1: Timer Interrupt Enabled fot Chan 0 */

   /*----------------------------------Time Set----------------------------------------*/

   LPIT0 ->TMR[0].TVAL = 40000000; /* Chan 0 Timeout period: 40M clocks */

   /* 40000000 => 1 sec
    * 80000000 => 2 sec
    */
   LPIT0 ->TMR[0].TCTRL |= LPIT_TMR_TCTRL_T_EN_MASK;
   /* T_EN=1: Timer channel is enabled */
   /* CHAIN=0: channel chaining is disabled */
   /* MODE=0: 32 periodic counter mode */
   /* TSOT=0: Timer decrements immediately based on restart */
   /* TSOI=0: Timer does not stop after timeout */
   /* TROT=0 Timer will not reload on trigger */
   /* TRG_SRC=0: External trigger soruce */
   /* TRG_SEL=0: Timer chan 0 trigger source is selected*/
}



