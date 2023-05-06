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

// variables to hold Joystick directions
extern volatile uint32_t PS2_X_DIR;
extern volatile uint32_t PS2_Y_DIR;

// variables to hold Accelerometer directions
extern volatile uint32_t ACCEL_X_DIR;
extern volatile uint32_t ACCEL_Y_DIR;

#endif /* INTERRUPTS_H_ */
