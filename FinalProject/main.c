/**
 * main.c
 *
 *  Created on: Apr 27, 2023
 *     Authors: Rusheel Dasari and Khiem Vu
 */

/*
 * Copyright (c) 2016-2019, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ======== main_freertos.c ========
 */
#include "msp.h"
#include "msp432p401r.h"
#include <stdint.h>
#include <stdio.h>

/* RTOS header files */
#include <FreeRTOS.h>
#include <task.h>

// Our header files
#include "music.h"
#include "our_tasks.h"
#include "peripherals.h"
#include "tetris.h"
#include "lcd.h"

/*
 *  ======== main ========
 */
int main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

    // initialize peripherals, game, i2c, etc.
	Crystalfontz128x128_Init();
	i2c_init();
	opt3001_init();
	peripherals_MKII_S1_init();
	peripherals_MKII_S2_init();
	peripherals_ADC14_PS2_ACCEL_XY();
	init_game();

	// define queues
    Queue_Game = xQueueCreate(1, sizeof(GameData*));
    Queue_Peripherals = xQueueCreate(1, sizeof(InputData));

    // create tasks
    xTaskCreate(task_music_buzzer, "Buzzer Music Task",
    configMINIMAL_STACK_SIZE,
                NULL, 1, &Task_Music_Buzzer_Handle);

    xTaskCreate(task_light_sensor, "Light Sensor Task",
    configMINIMAL_STACK_SIZE,
                NULL, 4, &Task_Light_Sensor_Handle);

    xTaskCreate(task_cycle_game, "Cycle Game Task",
    configMINIMAL_STACK_SIZE,
                NULL, 4, &Task_Cycle_Game_Handle);

    xTaskCreate(task_update_inputs_game, "Update Inputs Game Task",
    configMINIMAL_STACK_SIZE,
                NULL, 4, &Task_Update_Inputs_Game_Handle);

    xTaskCreate(task_screen_LCD, "LCD Screen Task",
    configMINIMAL_STACK_SIZE,
                NULL, 4, &Task_Screen_LCD_Handle);

    xTaskCreate(task_ADC_timer, "ADC Timer Task",
    configMINIMAL_STACK_SIZE,
                NULL, 3, &Task_ADC_Timer_Handle);

    xTaskCreate(task_ADC_bottom_half, "ADC Bottom Half Task",
    configMINIMAL_STACK_SIZE,
                NULL, 5, &Task_ADC_Handle);

    xTaskCreate(task_MKII_S1, "MKII S1 Task",
    configMINIMAL_STACK_SIZE,
                NULL, 4, &Task_MKII_S1_Handle);

    xTaskCreate(task_MKII_S2, "MKII S2 Task",
    configMINIMAL_STACK_SIZE,
                NULL, 4, &Task_MKII_S2_Handle);

    /* Start the FreeRTOS scheduler */
    vTaskStartScheduler();

    while (1)
    {
    }

    return (0);
}

//*****************************************************************************
//
//! \brief Application defined malloc failed hook
//!
//! \param  none
//!
//! \return none
//!
//*****************************************************************************
void vApplicationMallocFailedHook()
{
    /* Handle Memory Allocation Errors */
    while (1)
    {
    }
}

//*****************************************************************************
//
//! \brief Application defined stack overflow hook
//!
//! \param  none
//!
//! \return none
//!
//*****************************************************************************
void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName)
{
    //Handle FreeRTOS Stack Overflow
    while (1)
    {
    }
}
