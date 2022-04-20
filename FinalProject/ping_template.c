/**
 * Driver for ping sensor
 * @file ping.c
 * @author
 */

#include "ping_template.h"
#include "Timer.h"
#include "lcd.h"
#include "timer.h"


volatile unsigned long START_TIME = 0;
volatile unsigned long END_TIME = 0;
volatile enum{LOW, HIGH, DONE} STATE = LOW; // State of ping echo pulse

void ping_init (void){

    SYSCTL_RCGCGPIO_R |= 0b00010;
    SYSCTL_RCGCTIMER_R |= 0b1000;

    //wait
    while ((SYSCTL_PRGPIO_R & 0x2) == 0) {};
    while ((SYSCTL_PRTIMER_R & 0x8) == 0) {};

    //GPIO_PORTB_DIR_R &= ~0b1000;
    GPIO_PORTB_DEN_R |= 0b1000;
    GPIO_PORTB_AFSEL_R |= 0b1000; //changed
    GPIO_PORTB_PCTL_R = (GPIO_PORTB_PCTL_R & 0xFFFF0FFF)|0x07000;
    TIMER3_CTL_R &= ~0b100000000;
    TIMER3_CFG_R = 0x4;
    TIMER3_TBMR_R |= 0x7;
    TIMER3_TBMR_R &= ~0x10;
    TIMER3_CTL_R |= 0b110000000000;
    TIMER3_TBPR_R |= 0xFF;
    TIMER3_TBILR_R |= 0xFFFF;
    TIMER3_IMR_R |= 0x400;
    TIMER3_ICR_R |= 0x400;
    TIMER3_CTL_R |= 0b100000000;
    NVIC_EN1_R |= 0b10000;
    NVIC_PRI9_R &= ~0x20;
    NVIC_PRI9_R |= 0x20; //set priority for the interrupts

    IntRegister(INT_TIMER3B, TIMER3B_Handler);

    IntMasterEnable();

    // Configure and enable the timer
    // TIMER3_CTL_R |= 0b100000000;
}

void ping_trigger (void){
    STATE = LOW;
    // Disable timer and disable timer interrupt
    TIMER3_CTL_R &= ~0b100000000;
    TIMER3_IMR_R &= ~0x400; //0b 0100 0000 0000
    // Disable alternate function (disconnect timer from port pin)
    GPIO_PORTB_AFSEL_R &= ~0b1000;

    // YOUR CODE HERE FOR PING TRIGGER/START PULSE
    GPIO_PORTB_DIR_R |= 0b1000;
    GPIO_PORTB_DATA_R &= ~0b00001000;
    GPIO_PORTB_DATA_R |= ~0b11110111;
    timer_waitMicros(5);
    GPIO_PORTB_DATA_R &= ~0b00001000;
    // Clear an interrupt that may have been erroneously triggered
    TIMER3_ICR_R |= 0b010000000000;
    // Re-enable alternate function, timer interrupt, and timer
    GPIO_PORTB_AFSEL_R |= 0b1000;
    TIMER3_IMR_R |= 0x400;
    TIMER3_CTL_R |= 0b100000000;
}

void TIMER3B_Handler(void){
    if(TIMER3_MIS_R & 0b010000000000){
        TIMER3_ICR_R |= 0x400;
        if(STATE == LOW){
            START_TIME = TIMER3_TBR_R;
            STATE = HIGH;
        } else if(STATE == HIGH){
            END_TIME = TIMER3_TBR_R;
            STATE = DONE;
        } else {
            STATE = DONE;
        }
    }
}
//check interrupt
//clear interrupt
//check if waiting for rising edge event (if STATE = LOW)
//record starting time
//update STATE to high
//else if STATE = High
//update STATE to Low
//end time = TBR register
//update STATE with done


float ping_getDistance (void){

    int overflow = 0;
    double clocks = START_TIME - END_TIME;
    if(START_TIME < END_TIME){
        overflow += 1;
    }
    double distance = clocks * .002125 / 2;
    lcd_printf("clocks: %.0lf\ndistance: %.1lf\noverflow: %d",clocks,distance, overflow);

}
