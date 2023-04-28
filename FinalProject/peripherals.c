/*
 * peripherals.c
 *
 *  Created on: Apr 27, 2023
 *     Authors: Rusheel Dasari and Khiem Vu
 */

#include "peripherals.h"

/*
 * Busy waits for a given number of SMCLK clock cycles
 *
 * Parameters
 *      ticks   :   Number of ticks to wait
 * Returns
 *      None
 */
void peripherals_T32_1_wait(uint32_t ticks)
{
    // Stop the timer
    TIMER32_1->CONTROL = 0;

    // Set the timer to be a 32-bit, one-shot
    TIMER32_1->CONTROL = TIMER32_CONTROL_ONESHOT | TIMER32_CONTROL_SIZE;

    // Set the load register
    TIMER32_1->LOAD = ticks;

    // Start the timer
    TIMER32_1->CONTROL |= TIMER32_CONTROL_ENABLE;

    // Wait until it reaches 0
    while (TIMER32_1->VALUE != 0)
    {
        //timer is still counting, so wait
    }

}

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
void peripherals_MKII_Buzzer_Init(uint16_t ticks_period)
{
    // Set P2.7 to be a GPIO OUTPUT Pin
    P2->DIR |= BIT7;

    // the TIMERA PWM controller will control the buzzer on the MKII
    // P2.7 <--> TA0.4
    P2->SEL1 &= ~BIT7;
    P2->SEL0 |= BIT7;

    // Turn off TA0
    TIMER_A0->CTL = 0;

    // Set the period of the timer
    TIMER_A0->CCR[0] = ticks_period - 1; // -1 because 0 indexing

    // Configure BUZZER Duty Cycle to 50%
    TIMER_A0->CCR[4] = (ticks_period / 2) - 1;

    // Configure TA0.4 for RESET/SET Mode
    TIMER_A0->CCTL[4] = TIMER_A_CCTLN_OUTMOD_7;

    // Select the master clock as the timer source
    TIMER_A0->CTL = TIMER_A_CTL_SSEL__SMCLK;

}

/*****************************************************
 * Turns the Buzzer on
 *
 * Parameters
 *      None
 *
 * Returns
 *      None
 *****************************************************/
void peripherals_MKII_Buzzer_On(void)
{
    // Clear the current Mode Control Bits
    TIMER_A0->CTL &= ~TIMER_A_CTL_MC_MASK;

    // Set mode Control to UP and CLear the current count
    TIMER_A0->CTL |= TIMER_A_CTL_MC__UP | TIMER_A_CTL_CLR;
}

/*****************************************************
 * Turns the Buzzer off
 *
 * Parameters
 *      None
 *
 * Returns
 *      None
 *****************************************************/
void peripherals_MKII_Buzzer_Off(void)
{
    // Turn off the timer
    TIMER_A0->CTL &= ~TIMER_A_CTL_MC_MASK;
}




