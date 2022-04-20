/*
 * copied from lab7main.c
 *
 *  Created on: april 15, 2022
 *      Author: Lab group 8D
 */

#include "open_interface.h"
#include "cyBot_uart.h"
#include "uart-interrupt.h"
#include "Movement.h"
#include "adc.h"
#include "servo.h"
#include "ping_template.h"

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

    bool done = false;

    while(1){

        if(command_byte == 'w'){
          //  oi_setWheels(300,300); //move forward at full speed
            move_forward(sensor_data, 100);

            done = true;
        }
        if(command_byte == 'a'){
            turn_left(sensor_data, 90);
            done = true;
        }
        if(command_byte == 'x'){
            move_back(sensor_data, 100);
            done = true;
        }
        if(command_byte == 'd'){
            turn_right(sensor_data, 90);
            done = true;
        }
        if(command_byte == 'm'){
            scan(0, 180,sensor_data, objects);
            done = true;
            //This scan should also move to smallest
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

