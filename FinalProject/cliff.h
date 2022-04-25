/*
 * cliff.h
 *
 *  Created on: Apr 25, 2022
 *      Author: bmbates
 */

#ifndef CLIFF_H_
#define CLIFF_H_
#include "open_interface.h"
#include "uart-interrupt.h"
char tapeDetect(oi_t* sensor);
char cliffDetect(oi_t* sensor);



#endif /* CLIFF_H_ */
