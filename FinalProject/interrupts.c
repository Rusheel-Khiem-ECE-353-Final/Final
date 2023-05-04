/*
 * interrupts.c
 *
 *  Created on: Apr 27, 2023
 *     Authors: Rusheel Dasari and Khiem Vu
 */

#include "interrupts.h"
#include "our_tasks.h"

volatile uint32_t PS2_X_DIR = ((int) (1.65 / (3.3 / 4096)));
volatile uint32_t PS2_Y_DIR = ((int) (1.65 / (3.3 / 4096)));
volatile uint32_t ACCEL_X_DIR = ((int) (1.65 / (3.3 / 4096)));
volatile uint32_t ACCEL_Y_DIR = ((int) (1.65 / (3.3 / 4096)));

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

    vTaskNotifyGiveFromISR(Task_ADC_Handle, &xHigherPriorityTaskWoken);
}

//volatile bool LCD_UPDATEABLE = false; // global variable used to indicate that the LCD screen can be updated
//
///*****************************************************
// * Configures Timer32_1 to generate a periodic interrupt every 10ms
// *
// * Parameters
// *      None
// *
// * Returns
// *      None
// *****************************************************/
//void T32_1_Interrupt_10ms()
//{
//    // ticks = desired period / core clock period
//    // 20e-3/(1/3e6) = (3e6 * 20)/1000
//    uint32_t ticks = ((SystemCoreClock * 10) / 1000) - 1;
//
//    // Stop the timer
//    TIMER32_1->CONTROL = 0;
//
//    // Set the load register
//    TIMER32_1->LOAD = ticks;
//
//    // Enable the Timer32 interrupt in NVIC
//    NVIC_EnableIRQ(T32_INT1_IRQn);
//    NVIC_SetPriority(T32_INT1_IRQn, 0);
//
//    // Start Timer32 and enable interrupt
//    TIMER32_1->CONTROL |= TIMER32_CONTROL_ENABLE |   // turn timer on
//            TIMER32_CONTROL_MODE |     // periodic mode
//            TIMER32_CONTROL_SIZE |     // 32 bit timer
//            TIMER32_CONTROL_IE;        // enable interrupts
//}
//
///*****************************************************
// * Configures Timer32_2 to generate a periodic interrupt every 100ms
// *
// * Parameters
// *      None
// *
// * Returns
// *      None
// *****************************************************/
//void T32_2_Interrupt_100ms()
//{
//    // ticks = desired period / core clock period
//    // 20e-3/(1/3e6) = (3e6 * 20)/1000
//    uint32_t ticks = ((SystemCoreClock * 100) / 1000) - 1;
//
//    // Stop the timer
//    TIMER32_2->CONTROL = 0;
//
//    // Set the load register
//    TIMER32_2->LOAD = ticks;
//
//    // Enable the Timer32 interrupt in NVIC
//    NVIC_EnableIRQ(T32_INT2_IRQn);
//    NVIC_SetPriority(T32_INT2_IRQn, 1);
//
//    // Start Timer32 and enable interrupt
//    TIMER32_2->CONTROL |= TIMER32_CONTROL_ENABLE |   // turn timer on
//            TIMER32_CONTROL_MODE |     // periodic mode
//            TIMER32_CONTROL_SIZE |     // 32 bit timer
//            TIMER32_CONTROL_IE;        // enable interrupts
//}
//
///*****************************************************
// * ISR that handles when Timer32_1 generates an interrupt
// *
// * Parameters
// *      None
// *
// * Returns
// *      None
// *****************************************************/
//void T32_INT1_IRQHandler()
//{
//    // Start the ADC conversion
//    ADC14->CTL0 |= ADC14_CTL0_SC | ADC14_CTL0_ENC;
//
//    // Clear the timer interrupt
//    TIMER32_1->INTCLR = BIT0;
//}
//
///*****************************************************
// * ISR that handles when Timer32_2 generates an interrupt
// *
// * Parameters
// *      None
// *
// * Returns
// *      None
// *****************************************************/
//void T32_INT2_IRQHandler(void)
//{
//    // can now update the LCD after 100ms
//    LCD_UPDATEABLE = true;
//
//    // Clear the timer interrupt
//    TIMER32_2->INTCLR = BIT0;
//}

