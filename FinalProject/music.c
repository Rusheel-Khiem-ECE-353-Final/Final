/*
 * music.c
 *
 *  Created on: Apr 27, 2023
 *      Authors: Rusheel Dasari and Khiem Vu
 */

#include "music.h"
#include "our_tasks.h"

Note_t Song[] = {
        { NOTE_E6, ONE_QUARTER, true },  // Tone, Time, Delay
        { NOTE_B5, ONE_EIGHTH, true }, { NOTE_C6, ONE_EIGHTH, true }, {
                NOTE_D6, ONE_QUARTER, true },
        { NOTE_C6, ONE_EIGHTH, true }, { NOTE_B5, ONE_EIGHTH, true }, {
                NOTE_A5, ONE_QUARTER, true },
        { NOTE_A5, ONE_EIGHTH, true }, { NOTE_C6, ONE_EIGHTH, true }, {
                NOTE_E6, ONE_QUARTER, true },
        { NOTE_D6, ONE_EIGHTH, true }, { NOTE_C6, ONE_EIGHTH, true }, {
                NOTE_B5, THREE_EIGHTH, true },
        { NOTE_C6, ONE_EIGHTH, true }, { NOTE_D6, ONE_QUARTER, true }, {
                NOTE_E6, ONE_QUARTER, true },
        { NOTE_C6, ONE_QUARTER, true }, { NOTE_A5, ONE_QUARTER, true }, {
                NOTE_A5, ONE_QUARTER, true },
        { 0, ONE_QUARTER, true }, { 0, ONE_EIGHTH, true },
        { NOTE_D6, ONE_QUARTER, true }, { NOTE_F6, ONE_EIGHTH, true }, {
                NOTE_A6, ONE_QUARTER, true },
        { NOTE_G6, ONE_EIGHTH, true }, { NOTE_F6, ONE_EIGHTH, true }, {
                NOTE_E6, THREE_EIGHTH, true },
        { NOTE_C6, ONE_EIGHTH, true }, { NOTE_E6, ONE_QUARTER, true }, {
                NOTE_D6, ONE_EIGHTH, true },
        { NOTE_C6, ONE_EIGHTH, true }, { NOTE_B5, ONE_QUARTER, true }, {
                NOTE_B5, ONE_EIGHTH, true },
        { NOTE_C6, ONE_EIGHTH, true }, { NOTE_D6, ONE_QUARTER, true }, {
                NOTE_E6, ONE_QUARTER, true },
        { NOTE_C6, ONE_QUARTER, true }, { NOTE_A5, ONE_QUARTER, true }, {
                NOTE_A5, ONE_QUARTER, true },
        { 0, ONE_QUARTER, true }, { NOTE_E6, ONE_QUARTER, true }, { NOTE_B5,
                                                                    ONE_EIGHTH,
                                                                    true },
        { NOTE_C6, ONE_EIGHTH, true }, { NOTE_D6, ONE_QUARTER, true }, {
                NOTE_C6, ONE_EIGHTH, true },
        { NOTE_B5, ONE_EIGHTH, true }, { NOTE_A5, ONE_QUARTER, true }, {
                NOTE_A5, ONE_EIGHTH, true },
        { NOTE_C6, ONE_EIGHTH, true }, { NOTE_E6, ONE_QUARTER, true }, {
                NOTE_D6, ONE_EIGHTH, true },
        { NOTE_C6, ONE_EIGHTH, true }, { NOTE_B5, THREE_EIGHTH, true }, {
                NOTE_C6, ONE_EIGHTH, true },
        { NOTE_D6, ONE_QUARTER, true }, { NOTE_E6, ONE_QUARTER, true }, {
                NOTE_C6, ONE_QUARTER, true },
        { NOTE_A5, ONE_QUARTER, true }, { NOTE_A5, ONE_QUARTER, true }, {
                0, ONE_QUARTER, true },
        { 0, ONE_EIGHTH, true }, { NOTE_D6, ONE_QUARTER, true }, { NOTE_F6,
                                                                   ONE_EIGHTH,
                                                                   true },
        { NOTE_A6, ONE_QUARTER, true }, { NOTE_G6, ONE_EIGHTH, true }, {
                NOTE_F6, ONE_EIGHTH, true },
        { NOTE_E6, THREE_EIGHTH, true }, { NOTE_C6, ONE_EIGHTH, true }, {
                NOTE_E6, ONE_QUARTER, true },
        { NOTE_D6, ONE_EIGHTH, true }, { NOTE_C6, ONE_EIGHTH, true }, {
                NOTE_B5, ONE_QUARTER, true },
        { NOTE_B5, ONE_EIGHTH, true }, { NOTE_C6, ONE_EIGHTH, true }, {
                NOTE_D6, ONE_QUARTER, true },
        { NOTE_E6, ONE_QUARTER, true }, { NOTE_C6, ONE_QUARTER, true }, {
                NOTE_A5, ONE_QUARTER, true },
        { NOTE_A5, ONE_QUARTER, true }, { 0, ONE_QUARTER, true }, { NOTE_E5,
                                                                    ONE_HALF,
                                                                    true },
        { NOTE_C5, ONE_HALF, true }, { NOTE_D5, ONE_HALF, true }, { NOTE_B4,
                                                                    ONE_HALF,
                                                                    true },
        { NOTE_C5, ONE_HALF, true }, { NOTE_A4, ONE_HALF, true }, { NOTE_G4S,
                                                                    ONE_HALF,
                                                                    true },
        { NOTE_B4, ONE_HALF, true }, { NOTE_E5, ONE_HALF, true }, { NOTE_C5,
                                                                    ONE_HALF,
                                                                    true },
        { NOTE_D5, ONE_HALF, true }, { NOTE_B4, ONE_HALF, true }, { NOTE_C5,
                                                                    ONE_QUARTER,
                                                                    true },
        { NOTE_E5, ONE_QUARTER, true }, { NOTE_A5, ONE_QUARTER, true }, {
                NOTE_A5, ONE_QUARTER, true },
        { NOTE_G5S, THREE_QUARTER, true }, { 0, ONE_QUARTER, true } };

//***************************************************************
// This function returns how long an individual  notes is played
//***************************************************************
uint32_t music_get_time_delay(measure_time_t time)
{
    uint32_t time_return = 0;

    time_return = MEASURE_DURATION * MEASURE_RATIO;

    switch (time)
    {
    case ONE_QUARTER:
    {
        time_return = time_return / 4;
        break;
    }
    case ONE_HALF:
    {
        time_return = time_return / 2;
        break;
    }
    case THREE_QUARTER:
    {
        time_return = time_return * 3;
        time_return = time_return / 4;
        break;
    }
    case ONE_EIGHTH:
    {
        time_return = time_return / 8;
        break;
    }
    case THREE_EIGHTH:
    {
        time_return = time_return * 3;
        time_return = time_return / 8;
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
    // initialize the buzzer with the note
    peripherals_MKII_Buzzer_Init(Song[note_index].period);

    // start playing the note for the correct duration
    peripherals_MKII_Buzzer_On();
    peripherals_T32_1_wait(music_get_time_delay(Song[note_index].time));

    // stop playing the note
    peripherals_MKII_Buzzer_Off();

    // if delay needed between notes, add it
    if (Song[note_index].delay)
    {
        peripherals_T32_1_wait(DELAY_AMOUNT);
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
        while (!play_music)
        {
            if(started == false)
            {
                i = 0;
            }
        }
        music_play_note(i);
    }
}

