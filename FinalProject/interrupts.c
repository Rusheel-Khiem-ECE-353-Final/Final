/*
 * interrupts.c
 *
 *  Created on: Apr 27, 2023
 *     Authors: Rusheel Dasari and Khiem Vu
 */

#include "interrupts.h"
#include "our_tasks.h"

// variables to hold Joystick directions
volatile uint32_t PS2_X_DIR = ((int) (1.65 / (3.3 / 4096)));
volatile uint32_t PS2_Y_DIR = ((int) (1.65 / (3.3 / 4096)));

// variables to hold Accelerometer directions
volatile uint32_t ACCEL_X_DIR = ((int) (1.65 / (3.3 / 4096)));
volatile uint32_t ACCEL_Y_DIR = ((int) (1.65 / (3.3 / 4096)));

/*
 * Interrupt Service Routine to handle Analog-to-Digital conversions
 * for the Joystick and the Accelerometer
 */
void ADC14_IRQHandler()
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    // Read the PS2-X value
    PS2_X_DIR = ADC14->MEM[0];

    // Read the PS2-Y value
    PS2_Y_DIR = ADC14->MEM[1];

    // Read the ACCEL-X value
    ACCEL_X_DIR = ADC14->MEM[2];

    // Read the ACCEL-Y value
    ACCEL_Y_DIR = ADC14->MEM[3];

    // go to ADC bottom half
    vTaskNotifyGiveFromISR(Task_ADC_Handle, &xHigherPriorityTaskWoken);
}

