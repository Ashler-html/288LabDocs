/*
 * cliff.c
 *
 *  Created on: Apr 25, 2022
 *      Author: brodie bates
 */
#include "open_interface.h"
#include "uart-interrupt.h"
#include "sound.h"
char cliffDetect(oi_t* sensor){
    uint16_t R = sensor -> cliffRightSignal;
    uint16_t L = sensor -> cliffLeftSignal;
    uint16_t FR = sensor -> cliffFrontRightSignal;
    uint16_t FL = sensor -> cliffFrontLeftSignal;
    loadSoundCliff();
    if(R < 200){
        char top[] = "Cliff Right \r\n";
        int printi;
        for(printi = 0; printi < strlen(top);printi++){
            uart_sendChar(top[printi]);
        }
        return 'R';
    }
    if(L < 200){
        char top[] = "Cliff Left \r\n";
        int printi;
        for(printi = 0; printi < strlen(top);printi++){
            uart_sendChar(top[printi]);
        }
        return 'L';
    }
    if(FR < 200){
        char top[] = "Cliff Front Right \r\n";
        int printi;
        for(printi = 0; printi < strlen(top);printi++){
            uart_sendChar(top[printi]);
        }
        return 'F';
    }
    if(FL < 200){
        char top[] = "Cliff Front Left \r\n";
        int printi;
        for(printi = 0; printi < strlen(top);printi++){
            uart_sendChar(top[printi]);
        }
        return 'F';
    }
    return 'N';
}
char tapeDetect(oi_t* sensor){
    loadSoundCliff();
    uint16_t R = sensor -> cliffRightSignal;
    uint16_t L = sensor -> cliffLeftSignal;
    uint16_t FR = sensor -> cliffFrontRightSignal;
    uint16_t FL = sensor -> cliffFrontLeftSignal;
    if(R > 2650){
        char top[] = "Tape Right \r\n";
        int printi;
        for(printi = 0; printi < strlen(top);printi++){
            uart_sendChar(top[printi]);
        }
        return 'R';
    }
    if(L > 2650){
        char top[] = "Tape Left \r\n";
        int printi;
        for(printi = 0; printi < strlen(top);printi++){
            uart_sendChar(top[printi]);
        }
        return 'L';
    }
    if(FR > 2650){
        char top[] = "Tape Front Right \r\n";
        int printi;
        for(printi = 0; printi < strlen(top);printi++){
            uart_sendChar(top[printi]);
        }
        return 'F';
    }
    if(FL > 2650){
        char top[] = "Tape Front Left \r\n";
        int printi;
        for(printi = 0; printi < strlen(top);printi++){
            uart_sendChar(top[printi]);
        }
        return 'F';
    }
    return 'N';
}


