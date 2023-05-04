/*
 * music.h
 *
 *  Created on: Apr 27, 2023
 *     Authors: Rusheel Dasari and Khiem Vu
 */

#ifndef MUSIC_H_
#define MUSIC_H_

#include <stdint.h>
#include <stdbool.h>
#include "msp.h"
#include "peripherals.h"

//*****************************************************************************
// You will need to determine the number of SMCLKs it will
// take for the following notes and their associated frequencies.
//
// Assume that your MCU is running at 24MHz.
//
// My notes: Calculated using (24MHz / note_freq) - 1
//*****************************************************************************

#define NOTE_G4S       57789 // 415 Hz
#define NOTE_A4        54544 // 440 Hz
#define NOTE_A4S       51483 // 466 Hz
#define NOTE_B4        48594 // 494 Hz
#define NOTE_C5        45866 // 523 Hz
#define NOTE_C5S       43291 // 554 Hz
#define NOTE_D5        40862 // 587 Hz
#define NOTE_D5S       38569 // 622 Hz
#define NOTE_E5        36404 // 659 Hz
#define NOTE_F5        34360 // 698 Hz
#define NOTE_F5S       32432 // 740 Hz
#define NOTE_G5        30611 // 784 Hz
#define NOTE_G5S       28880 // 831 Hz
#define NOTE_A5        27272 // 880 Hz
#define NOTE_A5S       25750 // 932 Hz
#define NOTE_B5        24290 // 988 Hz
#define NOTE_C6        22944 // 1046 Hz
#define NOTE_C6S       21640 // 1109 Hz
#define NOTE_D6        20425 // 1175 Hz
#define NOTE_D6S       19276 // 1245 Hz
#define NOTE_E6        18195 // 1319 Hz
#define NOTE_F6        17179 // 1397 Hz
#define NOTE_F6S       16215 // 1480 Hz
#define NOTE_G6        15305 // 1568 Hz
#define NOTE_G6S       14448 // 1661 Hz
#define NOTE_A6        13635 // 1760 Hz
#define NOTE_A6S       12868 // 1865 Hz
#define NOTE_B6        12145 // 1976 Hz
#define NOTE_C7        11466 // 2093 Hz
#define NOTE_C7S       10824 // 2217 Hz
#define NOTE_D7        10216 // 2349 Hz
#define NOTE_D7S       9641 // 2489 Hz
#define NOTE_E7        9100 // 2637 Hz
#define NOTE_F7        8589 // 2794 Hz
#define NOTE_F7S       8107 // 2960 Hz
#define NOTE_G7        7652 // 3136 Hz
#define NOTE_G7S       7224 // 3322 Hz
#define NOTE_A7        6817 // 3520 Hz


//*****************************************************************************
// DO NOT MODIFY ANYTHING BELOW
//*****************************************************************************
#define MEASURE_DURATION 10000000    // 416.667 milliseconds
#define MEASURE_RATIO           4    // 4/4 time
#define DELAY_AMOUNT       240000    // 10  milliseconds
#define SONG_NUM_NOTES         98

typedef enum measure_time_t {
    ONE_QUARTER,
    ONE_HALF,
    THREE_QUARTER,
    ONE_EIGHTH,
    THREE_EIGHTH
} measure_time_t;

typedef struct{
    uint32_t period;
    measure_time_t time;
    bool delay;
}Note_t;

//***************************************************************
//***************************************************************
void music_init(void);

//***************************************************************
//***************************************************************
void music_play_song(void);

#endif /* MUSIC_H_ */
