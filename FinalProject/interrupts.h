/*
 * interrupts.h
 *
 *  Created on: Apr 27, 2023
 *     Authors: Rusheel Dasari and Khiem Vu
 */

#ifndef INTERRUPTS_H_
#define INTERRUPTS_H_

#include "msp.h"
#include <stdbool.h>
#include <stdint.h>

extern volatile bool LCD_UPDATEABLE; // global variable used to indicate that the LCD screen can be updated

/*****************************************************
 * Configures Timer32_1 to generate a periodic interrupt every 10ms
 *
 * Parameters
 *      None
 *
 * Returns
 *      None
 *****************************************************/
void T32_1_Interrupt_10ms(void);

/*****************************************************
 * Configures Timer32_2 to generate a periodic interrupt every 100ms
 *
 * Parameters
 *      None
 *
 * Returns
 *      None
 *****************************************************/
void T32_2_Interrupt_100ms(void);

/*****************************************************
 * ISR that handles when Timer32_1 generates an interrupt
 *
 * Parameters
 *      None
 *
 * Returns
 *      None
 *****************************************************/
void T32_INT1_IRQHandler(void);

/*****************************************************
 * ISR that handles when Timer32_2 generates an interrupt
 *
 * Parameters
 *      None
 *
 * Returns
 *      None
 *****************************************************/
void T32_INT2_IRQHandler(void);


#endif /* INTERRUPTS_H_ */
