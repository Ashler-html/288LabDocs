/*
 * servo.c
 *
 *  Created on: Apr 6, 2022
 *      Author: bmbates
 */
#include "timer.h"

uint32_t intoff = 8; //Intercept offset. Shifts to the left, multiplied by 100 in function
int32_t scaleroff = 6; //Degree scaler offset, fixes 180 degree point

void servo_init(void){
    //enable PB5 GPIO, set direction to output, Set to alternate function T1CCP1 (7), timer 1B
    SYSCTL_RCGCTIMER_R |= 0x2; //enable timer 1 AKA Timer B
    SYSCTL_RCGCGPIO_R |= 0x02; //enable clock port b pg 340
    while((SYSCTL_RCGCTIMER_R & 0x2) == 0){};// wait for clock
    while((SYSCTL_PRGPIO_R & 0x2) == 0){};// wait for clock
    GPIO_PORTB_DIR_R |= 0x20; //set pin as input
    GPIO_PORTB_DEN_R |= 0x20; //enable pin

    GPIO_PORTB_AFSEL_R |= 0x20; //enable alternate function pin 3B
    GPIO_PORTB_PCTL_R &= ~0xF00000;
    GPIO_PORTB_PCTL_R |= 0x700000; //select alternate function



    // Configure and enable the timer

    TIMER1_CTL_R &= ~0x500; //disable timer 1B and set bit 10 to 0 to reserve edges
    TIMER1_CTL_R |= 0x800; //Make sure bit 11 is 1, sets edges to be reserved
    TIMER1_CFG_R &= 0x0; //clearing
    TIMER1_CFG_R |= 0x4; //Selects 16 bit timer
    TIMER1_TBMR_R &= ~0x8; //enable PWM mode
    TIMER1_TBMR_R |= 0xA; //set edge time capture mode periodic timer mode

    TIMER1_TBILR_R |= 0xE200;// start value
    TIMER1_TBPR_R |= 0x04; //setting count direction to down, and pre-scaler
    TIMER1_CTL_R |= 0x100; //enable timer b

}
void servo_move(uint32_t degrees){
//    uint32_t match = (320000-(8000 + (89 * degrees)));
    uint32_t match = (320000 - ( (intoff * 100) + 28000- (degrees* (148 + scaleroff) ) ) );
    TIMER1_TBMATCHR_R = (match & 0xFFFF); //16 bits
    TIMER1_TBPMR_R = (match & 0xFF0000) >> 16; //pre-scaler
}

