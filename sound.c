/*
 * sound.c
 *
 *  Created on: Apr 27, 2022
 *      Author: ltureaud
 */
#include "open_interface.h"
#include "cyBot_uart.h"
#include "uart-interrupt.h"
#include "Movement.h"
#include "math.h"
#include "adc.h"
#include "ping_template.h"
#include "servo.h"

void loadSoundCliff(){
    int songIndex = 0;
    int numNotes = 3;
    unsigned char notes[3];
    notes[0] = 65;
    notes[1] = 65;
    notes[2] = 65;
    unsigned char *note = &notes[0];
    unsigned char durations[3];
    durations[0] = 32;
    durations[1] = 32;
    durations[2] = 32;
    unsigned char *duration = &durations[0];
    oi_loadSong(songIndex, numNotes, note, duration);
}

void loadSoundBump(){
    int songIndex = 1;
    int numNotes = 3;
        unsigned char notes[3];
        notes[0] = 67;
        notes[1] = 64;
        notes[2] = 55;
        unsigned char *note = &notes[0];
        unsigned char durations[3];
        durations[0] = 32;
        durations[1] = 32;
        durations[2] = 32;
    unsigned char *duration = &durations[0];
    oi_loadSong(songIndex, numNotes, note, duration);
}

void loadSoundFinish(){
    int songIndex = 2;
    int numNotes = 5;
        unsigned char notes[5];
        notes[0] = 60;
        notes[1] = 67;
        notes[2] = 72;
        notes[3] = 67;
        notes[4] = 60;
        unsigned char *note = &notes[0];
        unsigned char durations[5];
        durations[0] = 16;
        durations[1] = 16;
        durations[2] = 16;
        durations[3] = 16;
        durations[4] = 16;
    unsigned char *duration = &durations[0];
    oi_loadSong(songIndex, numNotes, note, duration);
}

void playSoundCliff(){
    oi_play_song(0);
}

void playSoundBump(){
    oi_play_song(1);
}

void playSoundFinish(){
    oi_play_song(2);
}


