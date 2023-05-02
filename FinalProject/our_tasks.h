/*
 * our_tasks.h
 *
 *  Created on: Apr 27, 2023
 *     Authors: Rusheel Dasari and Khiem Vu
 */

#ifndef OUR_TASKS_H_
#define OUR_TASKS_H_

#include "msp.h"
#include <stdint.h>
#include <stdio.h>

/* RTOS header files */
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>

extern TaskHandle_t Task_Music_Buzzer_Handle;
extern TaskHandle_t Task_Screen_LCD_Handle;
extern TaskHandle_t Task_Cycle_Game_Handle;
extern QueueHandle_t Queue_Game;

void task_music_buzzer(void *pvParameters);
void task_screen_LCD(void *pvParameters);

#endif /* OUR_TASKS_H_ */
