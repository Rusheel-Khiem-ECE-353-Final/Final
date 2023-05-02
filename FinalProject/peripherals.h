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
#include <msp.h>        // access to MSP device stuff
#include "i2c.h"        // use I2C to communicate with OPT3001

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

void ADC14_PS2_XY(void);
void ADC14_ACCEL_XY(void);


// OPT3001 ambient light sensor code
#define OPT3001_SLAVE_ADDRESS 0x44

#define OPT_INTERRUPT_PIN 11
#define RESULT_REG 0x00
#define CONFIG_REG 0x01
#define LOWLIMIT_REG 0x02
#define HIGHLIMIT_REG 0x03
#define MANUFACTUREID_REG 0x7E
#define DEVICEID_REG 0x7F

/******************************************************************************
 * Initialize the opt3001 ambient light sensor on the MKII.  This function assumes
 * that the I2C interface has already been configured to operate at 100KHz.
 ******************************************************************************/
void opt3001_init(void);

/******************************************************************************
 * Returns the current ambient light in lux
 ******************************************************************************/
float opt3001_read_lux(void);

/******************************************************************************
 * Changes the LCD screen's colors based on the lux
 *******************************************************************************/
void display_lux(float light);

#endif /* PERIPHERALS_H_ */
