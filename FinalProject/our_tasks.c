/*
 * our_tasks.c
 *
 *  Created on: Apr 27, 2023
 *     Authors: Rusheel Dasari and Khiem Vu
 */

#include "our_tasks.h"
#include "music.h"
#include "lcd.h"
#include "interrupts.h"
#include "tetris.h"
#include "peripherals.h"
#include "images.h"
#include <stdio.h>

// set Task Handles to NULL
TaskHandle_t Task_Music_Buzzer_Handle = NULL;
TaskHandle_t Task_Light_Sensor_Handle = NULL;
TaskHandle_t Task_Screen_LCD_Handle = NULL;
TaskHandle_t Task_Cycle_Game_Handle = NULL;
TaskHandle_t Task_Update_Inputs_Game_Handle = NULL;
TaskHandle_t Task_ADC_Handle = NULL;
TaskHandle_t Task_ADC_Timer_Handle = NULL;
TaskHandle_t Task_MKII_S1_Handle = NULL;
TaskHandle_t Task_MKII_S2_Handle = NULL;

// define Queues
QueueHandle_t Queue_Peripherals;
QueueHandle_t Queue_Game;

// global variables required for correct implementation
bool light_mode = true;
bool started = false;
bool over = false;
bool play_music = true;
float light_mode_threshold = 200.0; //1341900.0;
InputData inputs = { 0, false, 0, false, 0, false, false, false, false };

/*
 * Task that receives notification from ADC IRQ. Gives to Light Sensor Task
 */
void task_ADC_bottom_half(void *pvParameters)
{
	// allow both Joystick and Accelerometer directions to be changed
    static bool ps2_x_allowed = true;
    static bool accel_allowed = true;

    while (1)
    {
    	// take the task from ADC IRQ
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        // determine Joystick and Accerelometer directions
        if (ps2_x_allowed)
        {
            if (PS2_X_DIR > PS2_UPPER_THRESHOLD)
            {
                inputs.ps2_x = 1;
            }
            else if (PS2_X_DIR < PS2_LOWER_THRESHOLD)
            {
                inputs.ps2_x = -1;
            }
            else
            {
                inputs.ps2_x = 0;
            }
            inputs.ps2_x_allowed = true;
            ps2_x_allowed = false;
        }
        else if ((PS2_X_DIR < PS2_UPPER_THRESHOLD)
                && (PS2_X_DIR > PS2_LOWER_THRESHOLD))
        {
            inputs.ps2_x = 0;
            ps2_x_allowed = true;
            inputs.ps2_x_allowed = true;
        }
        else
        {
            inputs.ps2_x_allowed = false;
        }

        if (accel_allowed)
        {
            if (ACCEL_X_DIR > ACCEL_UPPER_THRESHOLD)
            {
                inputs.accel_rotate = 1;
            }
            else if (ACCEL_X_DIR < ACCEL_LOWER_THRESHOLD)
            {
                inputs.accel_rotate = -1;
            }
            else
            {
                inputs.accel_rotate = 0;
            }
            inputs.rotate_allowed = true;
            accel_allowed = false;
        }
        else if ((ACCEL_X_DIR < ACCEL_UPPER_THRESHOLD)
                && (ACCEL_X_DIR > ACCEL_LOWER_THRESHOLD))
        {
            accel_allowed = true;
            inputs.accel_rotate = 0;
            inputs.rotate_allowed = true;
        }
        else
        {
            inputs.rotate_allowed = false;
        }

        if (PS2_Y_DIR > PS2_UPPER_THRESHOLD)
        {
            inputs.ps2_y = 1;
        }
        else if (PS2_Y_DIR < PS2_LOWER_THRESHOLD)
        {
            inputs.ps2_y = -1;
        }
        else
        {
            inputs.ps2_y = 0;
        }

        // give task
        xTaskNotifyGive(Task_Light_Sensor_Handle);
    }
}

/**
 * task for ADC timer, top-half for ADC bottom half task
 */
void task_ADC_timer(void *pvParameters)
{
    while (1)
    {
    	// enable ADC conversion
        ADC14->CTL0 |= ADC14_CTL0_SC | ADC14_CTL0_ENC;

        // wait 10 ms
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

/**
 * task for S1, received from light sensor and gives to S2 task
 */
void task_MKII_S1(void *pvParameters)
{
	// allow s1 to be changed
    static bool s1_allowed = true;

    while (1)
    {
    	// take task from light sensor
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        // determine if S1 is pressed and perform correct action
        if (peripherals_MKII_S1() && s1_allowed)
        {
            inputs.s1_pressed = true;
            inputs.s1_allowed = true;
            s1_allowed = false;
        }
        else if (!peripherals_MKII_S1())
        {
            inputs.s1_pressed = false;
            inputs.s1_allowed = true;
            s1_allowed = true;
        }
        else
        {
            inputs.s1_pressed = false;
            inputs.s1_allowed = false;
        }

        // give from task
        xTaskNotifyGive(Task_MKII_S2_Handle);
    }
}

/**
 * task for S2. receive from S1 and give to game-inputs task
 */
void task_MKII_S2(void *pvParameters)
{
    static bool s2_allowed = true;

    while (1)
    {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        if (peripherals_MKII_S2() && s2_allowed)
        {
            inputs.s2_pressed = true;
            inputs.s2_allowed = true;
            s2_allowed = false;
        }
        else if (!peripherals_MKII_S2())
        {
            inputs.s2_pressed = false;
            inputs.s2_allowed = true;
            s2_allowed = true;
        }
        else
        {
            inputs.s2_pressed = false;
            inputs.s2_allowed = false;
        }

        // send queue to change game inputs
        xQueueSendToBack(Queue_Peripherals, &inputs, portMAX_DELAY);
    }
}

/**
 * music task. play after all other tasks have run
 */
void task_music_buzzer(void *pvParameters)
{
    while (1)
    {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        music_play_song();
    }
}

/*
 * light sensor task. read value and change color palette
 * take from ADC bottom half and give
 * to S1
 */
void task_light_sensor(void *pvParameters)
{
    float lux;
    while (1)
    {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        lux = opt3001_read_lux();
        //printf("%f", lux);

        if (lux >= light_mode_threshold)
        {
            light_mode = true;
        }
        else
        {
            light_mode = false;
        }
        xTaskNotifyGive(Task_MKII_S1_Handle);
    }
}

/**
 * LCD task. display screen with correct inputs
 */
void task_screen_LCD(void *pvParameters)
{
    static bool invert = true;
    static bool background = false;
    static bool refreshed_started = false;
    static bool refreshed_over = false;

    while (1)
    {
        GameData *game;
        xQueueReceive(Queue_Game, &game, portMAX_DELAY);

        started = game->started;
        over = game->over;

        // determine if splash screens should be drawn
        if (!started || over)
        {

            background = false;
            play_music = false;
            if (!started)
            {
                if (refreshed_started == false)
                {
                    lcd_draw_rectangle(132 / 2 - 1, 132 / 2 - 1, 132, 132,
                    LCD_COLOR_BLACK);
                    refreshed_started = true;
                }
                lcd_draw_image(132 / 2 - 1, 132 / 2 - 1, TITLE_WIDTH,
                TITLE_HEIGHT,
                               Bitmaps_Title, light_mode ? LCD_COLOR_GREEN : LCD_COLOR_MAGENTA,
                               LCD_COLOR_BLACK);
            }
            else
            {
                refreshed_started = false;
            }
            if (over)
            {
                if (refreshed_over == false)
                {
                    lcd_draw_rectangle(132 / 2 - 1, 132 / 2 - 1, 132, 132,
                    LCD_COLOR_BLACK);
                    refreshed_over = true;
                }
                lcd_draw_image(132 / 2 - 1, 132 / 2 - 1, END_WIDTH, END_HEIGHT,
                               Bitmaps_GameOver, LCD_COLOR_RED,
                               LCD_COLOR_BLACK);
            }
            else
            {
                refreshed_over = false;
            }
            continue;
        }

        if (invert != light_mode || background == false)
        {
            background = true;
            invert = light_mode;
            if (light_mode)
            {
                lcd_draw_rectangle(132 / 2 - 1, 132 / 2 - 1, 132, 132,
                LCD_COLOR_WHITE);

                // black borders for hold and next
                lcd_draw_rectangle(3 * 132 / 4 - 1, 132 / 4 - 1, 6 * BLOCK_SIZE,
                                   6 * BLOCK_SIZE, LCD_COLOR_BLACK);
                lcd_draw_rectangle(3 * 132 / 4 - 1, 3 * 132 / 4 - 1,
                                   6 * BLOCK_SIZE, 6 * BLOCK_SIZE,
                                   LCD_COLOR_BLACK);
            }
            else
            {
                lcd_draw_rectangle(132 / 2 - 1, 132 / 2 - 1, 132, 132,
                LCD_COLOR_BLACK);

                // black borders for hold and next
                lcd_draw_rectangle(3 * 132 / 4 - 1, 132 / 4 - 1, 6 * BLOCK_SIZE,
                                   6 * BLOCK_SIZE, LCD_COLOR_WHITE);
                lcd_draw_rectangle(3 * 132 / 4 - 1, 3 * 132 / 4 - 1,
                                   6 * BLOCK_SIZE, 6 * BLOCK_SIZE,
                                   LCD_COLOR_WHITE);
            }
        }

        // determine if should play music
        if (game->paused == true || game->over == true || game->started != true)
        {
            play_music = false;
        }
        else
        {
            play_music = true;
        }

        int lcd_row = 0;
        int lcd_col = 0;
        int game_row = 19;
        int game_col = 0;

        int draw_row;
        int draw_col;
        Block block;
        uint16_t block_color;

        // draw left side of board (game board and pieces)
        // black game board
        lcd_draw_rectangle(132 / 4 - 1, 132 / 2 - 10, 132 / 2 - 6, 132,
                           light_mode ? LCD_COLOR_BLACK : LCD_COLOR_WHITE);

        // draw blocks/pieces
        for (lcd_row = 0; lcd_row < 20; lcd_row++)
        { // height
            for (lcd_col = 0; lcd_col < 10; lcd_col++)
            { // width

                game_row = 19 - lcd_row;
                game_col = lcd_col;

                draw_row = (lcd_row * 6) + 6 - 1;
                draw_col = (lcd_col * 6) + 6 - 1;

                block = game->board[game_row][game_col];

                if (block.empty)
                {
                    continue;
                }

                if (light_mode)
                {
                    switch (block.type)
                    {
                    case LINE:
                        block_color = LCD_COLOR_CYAN;
                        break;

                    case SQUARE:
                        block_color = LCD_COLOR_YELLOW;
                        break;

                    case J:
                        block_color = LCD_COLOR_BLUE;
                        break;

                    case L:
                        block_color = LCD_COLOR_ORANGE;
                        break;

                    case S:
                        block_color = LCD_COLOR_GREEN;
                        break;

                    case Z:
                        block_color = LCD_COLOR_RED;
                        break;

                    case T:
                        block_color = LCD_COLOR_MAGENTA;
                        break;
                    }
                }
                else
                {
                    switch (block.type)
                    {
                    case LINE:
                        block_color = LCD_COLOR_RED;
                        break;

                    case SQUARE:
                        block_color = LCD_COLOR_BLUE;
                        break;

                    case J:
                        block_color = LCD_COLOR_YELLOW;
                        break;

                    case L:
                        block_color = LCD_COLOR_BLUE2;
                        break;

                    case S:
                        block_color = LCD_COLOR_MAGENTA;
                        break;

                    case Z:
                        block_color = LCD_COLOR_CYAN;
                        break;

                    case T:
                        block_color = LCD_COLOR_GREEN;
                        break;
                    }
                }

                lcd_draw_rectangle(draw_col, draw_row, BLOCK_SIZE,
                BLOCK_SIZE,
                                   block_color);

            }
        }

        // draw the falling (current) piece
        if (game->over == false)
        {
            int i = 0;
            for (i = 0; i < 4; i++)
            {
                block = game->current->blocks[i];
                draw_row =
                        (int) ((19 - (block.y_offset + game->current->y)) * 6)
                                + 6 - 1;
                draw_col = (int) ((block.x_offset + game->current->x) * 6) + 6
                        - 1;

                if (light_mode)
                {
                    switch (block.type)
                    {
                    case LINE:
                        block_color = LCD_COLOR_CYAN;
                        break;

                    case SQUARE:
                        block_color = LCD_COLOR_YELLOW;
                        break;

                    case J:
                        block_color = LCD_COLOR_BLUE;
                        break;

                    case L:
                        block_color = LCD_COLOR_ORANGE;
                        break;

                    case S:
                        block_color = LCD_COLOR_GREEN;
                        break;

                    case Z:
                        block_color = LCD_COLOR_RED;
                        break;

                    case T:
                        block_color = LCD_COLOR_MAGENTA;
                        break;
                    }
                }
                else
                {
                    switch (block.type)
                    {
                    case LINE:
                        block_color = LCD_COLOR_RED;
                        break;

                    case SQUARE:
                        block_color = LCD_COLOR_BLUE;
                        break;

                    case J:
                        block_color = LCD_COLOR_YELLOW;
                        break;

                    case L:
                        block_color = LCD_COLOR_BLUE2;
                        break;

                    case S:
                        block_color = LCD_COLOR_MAGENTA;
                        break;

                    case Z:
                        block_color = LCD_COLOR_CYAN;
                        break;

                    case T:
                        block_color = LCD_COLOR_GREEN;
                        break;
                    }
                }

                lcd_draw_rectangle(draw_col, draw_row, BLOCK_SIZE,
                BLOCK_SIZE,
                                   block_color);
            }
        }

        // draw the right side of the board (hold and next pieces)
        // gray interior boxes for hold and next
        lcd_draw_rectangle(3 * 132 / 4 - 1, 132 / 4 - 1, 5 * BLOCK_SIZE,
                           5 * BLOCK_SIZE, LCD_COLOR_GRAY);
        lcd_draw_rectangle(3 * 132 / 4 - 1, 3 * 132 / 4 - 1, 5 * BLOCK_SIZE,
                           5 * BLOCK_SIZE, LCD_COLOR_GRAY);

        int i = 0;

        // draw hold piece
        if (game->held->type != EMPTY)
        {
            for (i = 0; i < 4; i++)
            {
                block = game->held->blocks[i];
                draw_row = (int) ((block.y_offset * -6 + (3 * 132 / 4 - 1)));
                draw_col = (int) ((block.x_offset * 6 + (3 * 132 / 4 - 1)));

                if (light_mode)
                {
                    switch (block.type)
                    {
                    case LINE:
                        block_color = LCD_COLOR_CYAN;
                        break;

                    case SQUARE:
                        block_color = LCD_COLOR_YELLOW;
                        break;

                    case J:
                        block_color = LCD_COLOR_BLUE;
                        break;

                    case L:
                        block_color = LCD_COLOR_ORANGE;
                        break;

                    case S:
                        block_color = LCD_COLOR_GREEN;
                        break;

                    case Z:
                        block_color = LCD_COLOR_RED;
                        break;

                    case T:
                        block_color = LCD_COLOR_MAGENTA;
                        break;
                    }
                }
                else
                {
                    switch (block.type)
                    {
                    case LINE:
                        block_color = LCD_COLOR_RED;
                        break;

                    case SQUARE:
                        block_color = LCD_COLOR_BLUE;
                        break;

                    case J:
                        block_color = LCD_COLOR_YELLOW;
                        break;

                    case L:
                        block_color = LCD_COLOR_BLUE2;
                        break;

                    case S:
                        block_color = LCD_COLOR_MAGENTA;
                        break;

                    case Z:
                        block_color = LCD_COLOR_CYAN;
                        break;

                    case T:
                        block_color = LCD_COLOR_GREEN;
                        break;
                    }
                }

                lcd_draw_rectangle(draw_col, draw_row, BLOCK_SIZE,
                BLOCK_SIZE,
                                   block_color);
            }
        }

        // draw next piece
        for (i = 0; i < 4; i++)
        {
            block = game->next->blocks[i];
            draw_row = (int) ((block.y_offset * -6 + (132 / 4 - 1)));
            draw_col = (int) ((block.x_offset * 6 + (3 * 132 / 4 - 1)));

            if (light_mode)
            {
                switch (block.type)
                {
                case LINE:
                    block_color = LCD_COLOR_CYAN;
                    break;

                case SQUARE:
                    block_color = LCD_COLOR_YELLOW;
                    break;

                case J:
                    block_color = LCD_COLOR_BLUE;
                    break;

                case L:
                    block_color = LCD_COLOR_ORANGE;
                    break;

                case S:
                    block_color = LCD_COLOR_GREEN;
                    break;

                case Z:
                    block_color = LCD_COLOR_RED;
                    break;

                case T:
                    block_color = LCD_COLOR_MAGENTA;
                    break;
                }
            }
            else
            {
                switch (block.type)
                {
                case LINE:
                    block_color = LCD_COLOR_RED;
                    break;

                case SQUARE:
                    block_color = LCD_COLOR_BLUE;
                    break;

                case J:
                    block_color = LCD_COLOR_YELLOW;
                    break;

                case L:
                    block_color = LCD_COLOR_BLUE2;
                    break;

                case S:
                    block_color = LCD_COLOR_MAGENTA;
                    break;

                case Z:
                    block_color = LCD_COLOR_CYAN;
                    break;

                case T:
                    block_color = LCD_COLOR_GREEN;
                    break;
                }
            }

            lcd_draw_rectangle(draw_col, draw_row, BLOCK_SIZE,
            BLOCK_SIZE,
                               block_color);
        }

        // give to music buzzer task
        xTaskNotifyGive(Task_Music_Buzzer_Handle);
    }
}
