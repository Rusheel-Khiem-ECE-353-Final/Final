/*
 * peripherals.c
 *
 *  Created on: Apr 27, 2023
 *     Authors: Rusheel Dasari and Khiem Vu
 */

#include "peripherals.h"
#include "math.h"

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


// OPT3001 Ambient Light Sensor Code

/******************************************************************************
 * Initialize the opt3001 ambient light sensor on the MKII.  This function assumes
 * that the I2C interface has already been configured to operate at 100KHz.
 ******************************************************************************/

void opt3001_init(void)
{
    // Initialize OPT3001
    /* ADD CODE */
    //interrupt:    J1.8 --> P4.6
    //SCL:          J1.9 --> P6.5
    //SDA:          J1.10 -> P6.4
    //0b1100010000010000 = 0xC410
    i2c_write_16(OPT3001_SLAVE_ADDRESS, CONFIG_REG, 0xC410);
}


/******************************************************************************
 * Returns the current ambient light in lux
 *  ******************************************************************************/
float opt3001_read_lux(void)
{
    // Read the Result register of OPT3001 and convert into Lux, then return.
    uint16_t result = i2c_read_16(OPT3001_SLAVE_ADDRESS, RESULT_REG);
    float lux = 0.01 * pow(2, (result >> 12)) * (result & 0x0FFF);

    return lux; // Need to modify

}

/******************************************************************************
 * Changes the LCD screen's colors based on the lux
 *******************************************************************************/
void display_lux(float light)
{
    //TODO: change colors of screen (invert colors maybe?)
    if(light < 200.0)
    {
//        ece353_MKII_RGB_LED(false, false, true); // turn on BLUE
    }
    else if (light < 500.0)
    {
//        ece353_MKII_RGB_LED(false, true, false); // turn on GREEN
    }
    else
    {
//        ece353_MKII_RGB_LED(true, false, false); // turn on RED
    }

    // example code to run in main (probably put in task)
//    float lux = 0.0;
//
//        WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
//
//        ece353_MKII_RGB_IO_Init(false);
//        i2c_init();
//        opt3001_init();
//
//        while(1){
//            lux = opt3001_read_lux();
//            display_lux(lux);
//        }
}


