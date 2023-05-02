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

TaskHandle_t Task_Music_Buzzer_Handle = NULL;
TaskHandle_t Task_Screen_LCD_Handle = NULL;

void task_music_buzzer(void *pvParameters)
{
    while (1)
    {
        music_play_song();
    }
}

void task_screen_LCD(void *pvParameters)
{
    while (1)
    {
        if (LCD_UPDATEABLE)
        {
            int lcd_row = 0;
            int lcd_col = 0;
            int game_row = 19;
            int game_col = 0;

            int draw_row;
            int draw_col;
            Block block;
            uint16_t block_color;
            for (lcd_row = 0; lcd_row < 20; lcd_row++)
            { // height
                for (lcd_col = 0; lcd_col < 10; lcd_col++)
                { // width

                    game_row = 19 - i;
                    game_col = j;

                    draw_row = (lcd_row * 6) + 3;
                    draw_col = (lcd_col * 6) + 3;

                    block = game->board[game_row][game_col];

                    if (block.empty)
                    {
                        continue;
                    }

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

                    lcd_draw_rectangle(draw_col, draw_row, BLOCK_SIZE,
                                       BLOCK_SIZE, block_color);

                }
            }

            if (game->over == false)
            {
                int i = 0;
                for (i = 0; i < 4; i++)
                {
                    block = game->current->blocks[i];
                    draw_row = (int)(block.y_offset + game->current->y) * 6 + 3;
                    draw_col = (int)(block.x_offset + game->current->x) * 6 + 3;

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

                    lcd_draw_rectangle(draw_col, draw_row, BLOCK_SIZE,
                                       BLOCK_SIZE, block_color);
                }
            }
        }
    }
}
