/*
 * cliff.c
 *
 *  Created on: Apr 25, 2022
 *      Author: brodie bates
 */
#include "open_interface.h"
#include "uart-interrupt.h"
char cliffDetect(oi_t* sensor){
    uint16_t R = sensor -> cliffRightSignal;
    uint16_t L = sensor -> cliffLeftSignal;
    uint16_t FR = sensor -> cliffFrontRightSignal;
    uint16_t FL = sensor -> cliffFrontLeftSignal;
    if(FR < 200){
        return 'R';
    }
    if(FL < 200){
        return 'L';
    }
    if(R < 200){
        return 'R';
    }
    if(L < 200){
        return 'L';
    }
    if(FR < 200 || FL < 200){
        return 'F';
    }
    return 'N';
}
char tapeDetect(oi_t* sensor){
   //char str[20];
    uint16_t R = sensor -> cliffRightSignal;
    uint16_t L = sensor -> cliffLeftSignal;
    uint16_t FR = sensor -> cliffFrontRightSignal;
    uint16_t FL = sensor -> cliffFrontLeftSignal;

    if(R > 2750){
        return 'R';
    }
    if(L > 2750){
        return 'L';
    }
    if(FR > 2500 || FL > 2500){
        return 'F';
    }
    return 'N';
}
