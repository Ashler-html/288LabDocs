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

void loadSoundSong(){
    int songIndex = 3;
    int numNotes = 20;
    unsigned char notes[24];
    notes[0] = 73;
    notes[1] = 73;
    notes[2] = 73;
    notes[3] = 73;
    notes[4] = 73;
    notes[5] = 71;
    notes[6] = 76;
    notes[7] = 73;
    notes[8] = 73;
    notes[9] = 73;
    notes[10] = 73;
    notes[11] = 73;
    notes[12] = 71;
    notes[13] = 76;
    notes[14] = 0;
    notes[15] = 71;
    notes[16] = 78;
    notes[17] = 78;
    notes[18] = 78;
    notes[19] = 78;
    notes[20] = 78;
    notes[21] = 80;
    notes[22] = 82;
    notes[23] = 83;
    unsigned char *note = &notes[0];
    unsigned char durations[24];
    durations[0] = 30;
    durations[1] = 20;
    durations[2] = 20;
    durations[3] = 10;
    durations[4] = 20;
    durations[5] = 20;
    durations[6] = 240;
    durations[7] = 30;
    durations[8] = 20;
    durations[9] = 20;
    durations[10] = 10;
    durations[11] = 20;
    durations[12] = 20;
    durations[13] = 240;
    durations[14] = 10;
    durations[15] = 10;
    durations[16] = 10;
    durations[17] = 10;
    durations[18] = 20;
    durations[19] = 10;
    durations[20] = 30;
    durations[21] = 20;
    durations[22] = 10;
    durations[23] = 50;


    unsigned char *duration = &durations[0];
    oi_loadSong(songIndex, numNotes, note, duration);
}

void playSoundSong(){
    oi_play_song(3);
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


