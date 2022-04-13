/*
 * movement.h
 *
 *  Created on: Feb 2, 2022
 *      Author: ltureaud
 */

#ifndef MOVEMENT_H_
#define MOVEMENT_H_

#include "Timer.h"
#include "lcd.h"
#include "open_interface.h"

void turn_right(oi_t *sensor, double degrees);

void turn_left(oi_t *sensor, double degrees);

int move_forward(oi_t *sensor, double distance);

void move_backwards(oi_t *sensor, double distance);

#endif /* MOVEMENT_H_ */
