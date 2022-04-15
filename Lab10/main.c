/*
 * main.c
 *
 *  Created on: Apr 6, 2022
 *      Author: bmbates
 */
#include "servo.h"
#include "timer.h"

int main(void){
    timer_init(); // Must be called before lcd_init(), which uses timer functions
    servo_init();
    timer_waitMillis(400);
    servo_move(0);
    timer_waitMillis(400);
    servo_move(45);
    timer_waitMillis(400);
    servo_move(90);
    timer_waitMillis(400);
    servo_move(135);
    timer_waitMillis(400);
    servo_move(180);

    return 0;
}



