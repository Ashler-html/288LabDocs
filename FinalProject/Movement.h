/*
 * Movement.h
 *
 *  Created on: Feb 2, 2022
 *      Author: bmbates
 */

#ifndef MOVEMENT_H_
#define MOVEMENT_H_
#include "open_interface.h"
#include "cyBot_uart.h"
#include "uart-interrupt.h"

typedef struct{
       int angle;
       double distance;
       double width;
} Object;


void move_to(Object obstacles[], oi_t* sensor, int objects);

bool Collision(oi_t *sensor);

void scan(int start, int end, oi_t *sensor, Object a[]);

double move_forward(oi_t *sensor_data, double distance_mm);

double move_back(oi_t *sensor_data, double distance_mm);

double turn_right(oi_t *sensor, double degrees);

double turn_left(oi_t *sensor, double degrees);




#endif /* MOVEMENT_H_ */
