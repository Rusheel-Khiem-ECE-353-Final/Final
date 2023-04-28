/*
 * peripherals.h
 *
 *  Created on: Apr 27, 2023
 *     Authors: Rusheel Dasari and Khiem Vu
 */

#ifndef PERIPHERALS_H_
#define PERIPHERALS_H_

// include libraries necessary for application
#include <stdbool.h>    // can return true/false
#include <stdint.h>     // access uint32 data types
#include <msp.h>      // access to MSP device stuff

/*
 * Busy waits for a given number of SMCLK clock cycles
 *
 * Parameters
 *      ticks   :   Number of ticks to wait
 * Returns
 *      None
 */
void peripherals_T32_1_wait(uint32_t ticks);

/*****************************************************
 * Sets the PWM period of the Buzzer. The duty cycle
 * will be set to 50%
 *
 * Parameters
 *      ticks_period    :   Period of PWM Pulse
 *
 * Returns
 *      None
 *****************************************************/
void peripherals_MKII_Buzzer_Init(uint16_t ticks_period);

/*****************************************************
 * Turns the Buzzer on
 *
 * Parameters
 *      None
 *
 * Returns
 *      None
 *****************************************************/
void peripherals_MKII_Buzzer_On(void);

/*****************************************************
 * Turns the Buzzer off
 *
 * Parameters
 *      None
 *
 * Returns
 *      None
 *****************************************************/
void peripherals_MKII_Buzzer_Off(void);

#endif /* PERIPHERALS_H_ */
