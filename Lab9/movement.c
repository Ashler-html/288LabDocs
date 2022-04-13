/*
 * movement.c
 *
 *  Created on: Feb 2, 2022
 *      Author: ltureaud
 */
#include "Timer.h"
#include "lcd.h"
#include "open_interface.h"
void turn_right(oi_t *sensor, double degrees){
    double sum = 0;
    oi_setWheels(-150,150); //move forward at full speed
    while (sum < degrees) {
             oi_update(sensor);
             sum += (-1 * sensor -> angle);


    }

}
void turn_left(oi_t *sensor, double degrees){
    double sum = 0;
    oi_setWheels(150,-150); //move forward at full speed
    while (sum < degrees) {
             oi_update(sensor);
             sum += sensor -> angle;

    }

}
int move_forward(oi_t *sensor, double distance){
    double sum = 0; // distance member in oi_t struct is type double
    oi_setWheels(150,150); //move forward at full speed
    while (sum < distance) {
        oi_update(sensor);
        if(sensor->bumpLeft){
            move_backwards(sensor, 150);
            turn_right(sensor, 75);
            move_forward(sensor, 250);
            turn_left(sensor, 75);
            sum -= 150;
            return 1;
        } else if(sensor->bumpRight){
            move_backwards(sensor, 150);
            turn_left(sensor, 75);
            move_forward(sensor, 250);
            turn_right(sensor, 75);
            sum -= 150;
            return 1;
        }
        sum += sensor -> distance; // use -> notation since pointer


    }
    return 0;
}

void move_backwards(oi_t *sensor, double distance){
    double sum = 0; // distance member in oi_t struct is type double
    oi_setWheels(-150,-150); //move forward at full speed
        while (sum < distance) {
         oi_update(sensor);
         sum += (-1 * sensor -> distance); // use -> notation since pointer
        }
}
