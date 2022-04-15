/**
 * Driver for ping sensor
 * @file ping.c
 * @author
 */

#include "ping_template.h"
#include "Timer.h"
#include "lcd.h"

volatile unsigned long START_TIME = 0;
volatile unsigned long END_TIME = 0;
volatile enum{LOW, HIGH, DONE} STATE = LOW; // State of ping echo pulse


unsigned int overflowNum = 0;
unsigned long clockPW = 0; //Clock pulse width
unsigned long msPW = 0; //ms Pulse width

void ping_init (void){

    SYSCTL_RCGCGPIO_R |= 0x02; //enable clock port b
    while((SYSCTL_PRGPIO_R & 0x2) == 0){};// wait for clock
    GPIO_PORTB_DIR_R &= ~0x8; //set pin as input
    GPIO_PORTB_DEN_R |= 0x8; //enable pin

    GPIO_PORTB_AFSEL_R |= 0x8; //enable alternate function pin 3B
    GPIO_PORTB_PCTL_R &= ~0xF000;
    GPIO_PORTB_PCTL_R |= 0x7000; //select alternate function



    // Configure and enable the timer
    SYSCTL_RCGCTIMER_R |= 0x8; //enable timer 3 AKA Timer B
    while(( SYSCTL_RCGCTIMER_R & 0x8) == 0){};// wait for clock
    TIMER3_CTL_R &= ~0x100; //clearing event edges
    TIMER3_CTL_R |= 0xC00; //set both edges as events
    TIMER3_CFG_R &= 0x0; //clearing
    TIMER3_CFG_R |= 0x4; //Selects 16 bit timer
    TIMER3_TBMR_R &= ~0x10; //enable capture mode
    TIMER3_TBMR_R |= 0x7; //set edge time capture mode

    TIMER3_TBILR_R |= 0xFFFF;// start value
    TIMER3_TBPR_R |= 0xFF; //setting count direction to down, and pre-scaler
    TIMER3_IMR_R |= 0x400; //enables interrupts
    TIMER3_ICR_R |= 0x400; //p754
    TIMER3_CTL_R |= 0x100; //enable timer b
    NVIC_EN1_R |= 0x10; //enable interrupts on NVIC register 5 pg 142
    NVIC_PRI9_R |= 0x20;//set priority for NVIC interrupts pg 154 pg 105  |=0x20  &= ~0x0E0

    IntRegister(INT_TIMER3B, TIMER3B_Handler); //enable interrupts

    IntMasterEnable();
}

void ping_trigger (void){
    STATE = LOW;
    // Disable timer and disable timer interrupt
    TIMER3_CTL_R &= ~0x100;//disable timer
    TIMER3_IMR_R &= ~0x400;//interrupt part
    // Disable alternate function (disconnect timer from port pin)
    GPIO_PORTB_AFSEL_R &= ~0x8; //disable alternate function
    GPIO_PORTB_DIR_R |= 0x8; //set pin as output
    while((GPIO_PORTB_DIR_R & 0x8) == 0){};// wait for enable

    //PING TRIGGER/START PULSE
    GPIO_PORTB_DATA_R &= ~0x8;
    timer_waitMicros(2);
    GPIO_PORTB_DATA_R |= 0x8;
    timer_waitMicros(5);
    GPIO_PORTB_DATA_R &= ~0x8;

    // Clear an interrupt that may have been erroneously triggered
    TIMER3_ICR_R |= 0x400; //pg 754 clears the CBERIS in RIS register and the CBEMIS in MIS register
    GPIO_PORTB_DIR_R &= ~0x8; //set pin as input
    // Re-enable alternate function, timer interrupt, and timer
    GPIO_PORTB_AFSEL_R |= 0x8;
    GPIO_PORTB_PCTL_R |= 0x7000;
    TIMER3_IMR_R |= 0x400;
    TIMER3_CTL_R |= 0x100;
}

void TIMER3B_Handler(void){
    if(TIMER3_MIS_R & 0x400){

      if(STATE == LOW){
          STATE = HIGH;
          START_TIME = TIMER3_TBR_R;  //pg 765
      }
      else if(STATE == HIGH){
          STATE = DONE;
          END_TIME = TIMER3_TBR_R;
      }
      TIMER3_ICR_R |=  0x400; //clearing interrupt register
    }
}

float ping_getDistance (void){
    ping_trigger();
    bool overflow;
    int overTrue = 0;
    lcd_printf("%lu %lu", START_TIME, END_TIME);
    while(STATE != DONE){}
    overflow = (START_TIME < END_TIME);
    if(overflow){ overTrue = 1; }
    else{ overTrue = 0; }
    clockPW = ((unsigned long) overflow << 24)  + (START_TIME - END_TIME);//deal with overflow and set clock pulse width
    msPW = clockPW * 0.0000625; //convert clock pulse to ms BAI pg 722
    lcd_printf("%lu %lu Overflow: %d", clockPW, msPW, overTrue);
    return (float) (msPW*34.3/2); //return pulse (ms) * 343 (m/s) / 2 for distance there and back
    //convert speed of sound to cm/ms
    /*
     * call ping_trigger
     *
     * wait here until both eddge happen \
     *
     * clocks into time
     * time into dist
     * return dist
     *
     */
    return -69;


}
