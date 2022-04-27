/*
 * copied from lab7main.c
 *
 *  Created on: april 15, 2022
 *      Author: Lab group 8D
 */

#include "open_interface.h"
#include "uart-interrupt.h"
#include "Movement.h"
#include "adc.h"
#include "servo.h"
#include "ping_template.h"
#include "sound.h"
#include "cliff.h"

extern volatile int command_flag;

extern volatile char command_byte;




int main(void){
    timer_init(); // Initialize Timer, needed before any LCD screen functions can be called
    // and enables time functions (e.g. timer_waitMillis)
    lcd_init();
    adc_init();
    uart_interrupt_init();
    ping_init();
    servo_init();
    servo_move(90);
    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);
    Object objects[5];
    int distanceMoved = 0;
    int xDistance = 0;
    int yDistance = 0;
    int direction = 90;
    bool done = false;
    loadSoundCliff();
    loadSoundBump();
    loadSoundFinish();
    while(1){
        if(command_byte == 'w'){
            //  oi_setWheels(300,300); //move forward at full speed
            distanceMoved = move_forward(sensor_data, 50);
            xDistance += distanceMoved * cos(direction * 3.1415926 / 180);
            yDistance += distanceMoved * sin(direction * 3.1415926 / 180);
            char str[] = "";
            sprintf(str, "\n\rDistance Moved: %d\n\rTotal Distance Moved:\n\rX-direction: %d\n\rY-Direction: %d\n\r", distanceMoved, xDistance, yDistance);
            int printi = 0;
            for(printi = 0; printi < strlen(str);printi++){
                uart_sendChar(str[printi]);
            }
            done = true;
        }
        if(command_byte == 'a'){
            direction += turn_left(sensor_data, 30);
            done = true;
        }
        if(command_byte == 'x'){
            distanceMoved = move_back(sensor_data, 50);
            xDistance +=  distanceMoved * cos(direction * 3.1415926 / 180);
            yDistance += distanceMoved * sin(direction * 3.1415926 / 180);
            char str[] = "";
            sprintf(str, "\n\rDistance Moved: %d\n\rTotal Distance Moved:\n\rX-direction: %d\n\rY-Direction: %d\n\r", distanceMoved, xDistance, yDistance);
            int printi = 0;
            for(printi = 0; printi < strlen(str);printi++){
                uart_sendChar(str[printi]);
            }
            done = true;
        }
        if(command_byte == 'd'){
            direction += turn_right(sensor_data, 30);
            done = true;
        }
        if(command_byte == 'm'){
            scan(0, 180,sensor_data, objects);
            done = true;
            //This scan should also move to smallest
        }
        if(command_byte == 'f'){
            playSoundFinish();
            done = true;
        }

        //        if(command_byte == '.'){
        //            break;
        //        }
        if(done){
            command_byte = ' ';
            oi_init(sensor_data);
            done = false;
        }
    }





    oi_free(sensor_data);
    return 0;
}

