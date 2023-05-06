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
#include <stdbool.h>

/* RTOS header files */
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>

#define PS2_UPPER_THRESHOLD     ((int)(2.55/(3.3/4096)))
#define PS2_LOWER_THRESHOLD     ((int)(0.75/(3.3/4096)))
#define ACCEL_UPPER_THRESHOLD   ((int)(1.9/(3.3/4096)))
#define ACCEL_LOWER_THRESHOLD   ((int)(1.3/(3.3/4096)))

struct InputData {
    int accel_rotate;
    bool rotate_allowed;
    int ps2_x;
    bool ps2_x_allowed;
    int ps2_y;
    bool s1_pressed;
    bool s1_allowed;
    bool s2_pressed;
    bool s2_allowed;

}typedef InputData;

extern bool play_music;
extern bool started;

extern TaskHandle_t Task_Music_Buzzer_Handle;
extern TaskHandle_t Task_Light_Sensor_Handle;
extern TaskHandle_t Task_Screen_LCD_Handle;
extern TaskHandle_t Task_Cycle_Game_Handle;
extern TaskHandle_t Task_Update_Inputs_Game_Handle;
extern TaskHandle_t Task_ADC_Handle;
extern TaskHandle_t Task_ADC_Timer_Handle;
extern TaskHandle_t Task_MKII_S1_Handle;
extern TaskHandle_t Task_MKII_S2_Handle;
extern QueueHandle_t Queue_Game;
extern QueueHandle_t Queue_Peripherals;

void task_music_buzzer(void *pvParameters);
void task_light_sensor(void *pvParameters);
void task_screen_LCD(void *pvParameters);
void task_ADC_bottom_half(void *pvParameters);
void task_ADC_timer(void *pvParameters);
void task_MKII_S1(void *pvParameters);
void task_MKII_S2(void *pvParameters);

#endif /* OUR_TASKS_H_ */
