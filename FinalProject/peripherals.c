/*
 * peripherals.c
 *
 *  Created on: Apr 27, 2023
 *     Authors: Rusheel Dasari and Khiem Vu
 */

#include "peripherals.h"
//#include "math.h"

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

/*
 void ADC14_PS2_XY()
 {
 // Configure the X direction as an analog input pin.
 // X = P6.0
 P6->SEL0 |= BIT0;
 P6->SEL1 |= BIT0;

 // Configure the Y direction as an analog input pin.
 // Y = P4.4
 P4->SEL0 |= BIT4;
 P4->SEL1 |= BIT4;

 // Configure CTL0 to sample 16-times in pulsed sample mode.
 ADC14->CTL0 = ADC14_CTL0_SHP | ADC14_CTL0_SHT02;

 // NEW -- Indicate that this is a sequence-of-channels.
 ADC14->CTL0 |= ADC14_CTL0_CONSEQ_1;

 // Configure ADC to return 12-bit values
 ADC14->CTL1 = ADC14_CTL1_RES_2;

 // Associate the X direction analog signal with MEM[0]
 ADC14->MCTL[0] = ADC14_MCTLN_INCH_15;

 // Associate the Y direction analog signal with MEM[1]
 ADC14->MCTL[1] = ADC14_MCTLN_INCH_9;

 // NEW -- Make sure to indicate this is the end of a sequence.
 ADC14->MCTL[1] |= ADC14_MCTLN_EOS;

 // Enable interrupts in the ADC AFTER a value is written into MEM[1].
 //
 // NEW: This is not the same as what is demonstrated in the example
 // coding video.
 ADC14->IER0 = ADC14_IER0_IE1;

 // Enable ADC Interrupt in the NVIC
 NVIC_EnableIRQ(ADC14_IRQn);

 // Turn ADC ON
 ADC14->CTL0 |= ADC14_CTL0_ON;
 }

 void ADC14_ACCEL_XY()
 {
 // Configure the X direction as an analog input pin.
 // X = P6.1
 P6->SEL0 |= BIT1;
 P6->SEL1 |= BIT1;

 // Configure the Y direction as an analog input pin.
 // Y = P4.0
 P4->SEL0 |= BIT0;
 P4->SEL1 |= BIT0;

 // Configure CTL0 to sample 16-times in pulsed sample mode.
 ADC14->CTL0 = ADC14_CTL0_SHP | ADC14_CTL0_SHT02;

 // NEW -- Indicate that this is a sequence-of-channels.
 ADC14->CTL0 |= ADC14_CTL0_CONSEQ_1;

 // Configure ADC to return 12-bit values
 ADC14->CTL1 = ADC14_CTL1_RES_2;

 // Associate the X direction analog signal with MEM[2]
 ADC14->MCTL[2] = ADC14_MCTLN_INCH_14;

 // Associate the Y direction analog signal with MEM[3]
 ADC14->MCTL[3] = ADC14_MCTLN_INCH_13;

 // NEW -- Make sure to indicate this is the end of a sequence.
 ADC14->MCTL[3] |= ADC14_MCTLN_EOS;

 // Enable interrupts in the ADC AFTER a value is written into MEM[3].
 //
 // NEW: This is not the same as what is demonstrated in the example
 // coding video.
 ADC14->IER0 = ADC14_IER0_IE3;

 // Enable ADC Interrupt in the NVIC
 NVIC_EnableIRQ(ADC14_IRQn);

 // Turn ADC ON
 ADC14->CTL0 |= ADC14_CTL0_ON;
 }
 */

void peripherals_ADC14_PS2_ACCEL_XY()
{
    // Configure the PS2-X direction as an analog input pin.
    // X = P6.0
    P6->SEL0 |= BIT0;
    P6->SEL1 |= BIT0;

    // Configure the PS2-Y direction as an analog input pin.
    // Y = P4.4
    P4->SEL0 |= BIT4;
    P4->SEL1 |= BIT4;

    // Configure the ACCEL-X direction as an analog input pin.
    // X = P6.1
    P6->SEL0 |= BIT1;
    P6->SEL1 |= BIT1;

    // Configure the ACCEL-Y direction as an analog input pin.
    // Y = P4.0
    P4->SEL0 |= BIT0;
    P4->SEL1 |= BIT0;

    // Configure CTL0 to sample 16-times in pulsed sample mode.
    ADC14->CTL0 = ADC14_CTL0_SHP | ADC14_CTL0_SHT02;

    // NEW -- Indicate that this is a sequence-of-channels.
    ADC14->CTL0 |= ADC14_CTL0_CONSEQ_1;

    // Configure ADC to return 12-bit values
    ADC14->CTL1 = ADC14_CTL1_RES_2;

    // Associate the PS2-X direction analog signal with MEM[0]
    ADC14->MCTL[0] = ADC14_MCTLN_INCH_15;

    // Associate the PS2-Y direction analog signal with MEM[1]
    ADC14->MCTL[1] = ADC14_MCTLN_INCH_9;

    // Associate the ACCEL-X direction analog signal with MEM[2]
    ADC14->MCTL[2] = ADC14_MCTLN_INCH_14;

    // Associate the ACCEL-Y direction analog signal with MEM[3]
    ADC14->MCTL[3] = ADC14_MCTLN_INCH_13;

    // NEW -- Make sure to indicate this is the end of a sequence.
    ADC14->MCTL[3] |= ADC14_MCTLN_EOS;

    // Enable interrupts in the ADC AFTER a value is written into MEM[3].
    ADC14->IER0 = ADC14_IER0_IE3;

    // Enable ADC Interrupt in the NVIC
    NVIC_EnableIRQ(ADC14_IRQn);
    NVIC_SetPriority(ADC14_IRQn, 33);

    // Turn ADC ON
    ADC14->CTL0 |= ADC14_CTL0_ON;
}

void peripherals_MKII_S1_init(void)
{
    P5->DIR &= ~BIT1;
    P5->REN |= BIT1;
    P5->OUT |= BIT1;
}

void peripherals_MKII_S2_init(void)
{
    P3->DIR &= ~BIT5;
    P3->REN |= BIT5;
    P3->OUT |= BIT5;

}

bool peripherals_MKII_S1(void)
{
    return ((!(P5->IN & BIT1 )) == 1);
}

bool peripherals_MKII_S2(void)
{
    return ((!(P3->IN & BIT5 )) == 1);
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
    //float lux = 0.01 * pow(2, (result >> 12)) * (result & 0x0FFF);
    float lux = 0.01 * (1 << (result >> 12)) * (result & 0x0FFF);

    return lux; // Need to modify

}

/******************************************************************************
 * Changes the LCD screen's colors based on the lux
 *******************************************************************************/
void display_lux(float light)
{
    //TODO: change colors of screen (invert colors maybe?)
    if (light < 200.0)
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

