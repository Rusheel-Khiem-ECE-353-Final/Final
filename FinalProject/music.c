/*
 * music.c
 *
 *  Created on: Apr 27, 2023
 *      Authors: Rusheel Dasari and Khiem Vu
 */

#include "music.h"

//TODO: change song to Tetris main theme
Note_t Song[] =
{
    {NOTE_C6,ONE_QUARTER,true},  // Tone, Time, Delay
    {NOTE_B5,ONE_QUARTER,true},
    {NOTE_D6,ONE_QUARTER,true},
    {NOTE_C6,ONE_QUARTER,true},
    {NOTE_F6,ONE_QUARTER,true},
    {NOTE_E6,ONE_QUARTER,true},
    {NOTE_G6,ONE_QUARTER,true},
    {NOTE_F6,ONE_QUARTER,true},
    {NOTE_A6,ONE_QUARTER,true},
    {NOTE_A6,ONE_QUARTER,true},
    {NOTE_A6,ONE_QUARTER,true},
    {NOTE_A6,ONE_QUARTER,true},
    {NOTE_A6,ONE_HALF,false},
    {NOTE_A6,ONE_HALF,true},
    {NOTE_G6,ONE_QUARTER,true},
    {NOTE_F6S,ONE_QUARTER,true},
    {NOTE_G6,ONE_QUARTER,true},
    {NOTE_A6,ONE_QUARTER,true},
    {NOTE_F6,ONE_QUARTER,true},
    {NOTE_G6,ONE_QUARTER,true},
    {NOTE_A6,ONE_QUARTER,true},
    {NOTE_F6,ONE_QUARTER,true},
    {NOTE_E6,ONE_QUARTER,true},
    {NOTE_D6,ONE_QUARTER,true},
    {NOTE_E6,ONE_QUARTER,true},
    {NOTE_F6,ONE_QUARTER,true},
    {NOTE_G6,ONE_HALF,false},
    {NOTE_G6,ONE_HALF,true}
};


//***************************************************************
// This function returns how long an individual  notes is played
//***************************************************************
uint32_t music_get_time_delay(measure_time_t time)
{
    uint32_t time_return = 0;

    time_return  = MEASURE_DURATION * MEASURE_RATIO;

    switch(time)
    {
        case ONE_QUARTER:
        {
            time_return  = time_return / 4;
            break;
        }
        case ONE_HALF:
        {
            time_return  = time_return / 2;
            break;
        }
        case ONE_EIGTH:
        {
            time_return  = time_return / 8;
            break;
        }
        case THREE_EIGTH:
        {
            time_return = time_return * 3;
            time_return  = time_return / 8;
            break;
        }
        default:
        {
            break;
        }
    }

    return time_return - DELAY_AMOUNT;

}


//***************************************************************************
// Plays a single note of the song based on note_index.  After
// the note is played, there is an optional delay in between
// notes.
//
// Examples
// Song[note_index].period       -- Used to determine the period of the
//                                  PWM pulse.
//
// Song[note_index].time         -- 1/4 or 1/2 time note.  Call
//                                  music_get_time_delay() to determine how
//                                  long to play the note
//
// Song[note_index].delay        -- If true, add a period of silence for
//                                  DELAY_AMOUNT
//
//                                  If false, return without delaying.
//*************************************************************************
static void music_play_note(uint16_t note_index)
{
    // TODO: change ece353 functions to peripherals functions
    // initialize the buzzer with the note
    ece353_MKII_Buzzer_Init(Song[note_index].period);

    // start playing the note for the correct duration
    ece353_MKII_Buzzer_On();
    ece353_T32_1_wait(music_get_time_delay(Song[note_index].time));

    // stop playing the note
    ece353_MKII_Buzzer_Off();

    // if delay needed between notes, add it
    if (Song[note_index].delay) {
        ece353_T32_1_wait(DELAY_AMOUNT);
    }
}


//***************************************************************
// Plays the song (loop through, playing each note)
// and then returns
//***************************************************************
void music_play_song(void)
{
    int i;
    for (i = 0; i < SONG_NUM_NOTES; i++)
    {
        music_play_note(i);
    }
}



