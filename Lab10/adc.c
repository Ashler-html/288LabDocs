/*
 * adc.c
 *
 *  Created on: Mar 23, 2022
 *      Author: bmbates
 */
#include <inc/tm4c123gh6pm.h>
#include <stdint.h>
#include <stdbool.h>
#include "Timer.h"

void adc_init(void){
    SYSCTL_RCGCADC_R |= 0x01;
    SYSCTL_RCGCGPIO_R |= 0x02;
    while((SYSCTL_PRGPIO_R & 0x02) != 0x02){};
    GPIO_PORTB_DIR_R &= ~0x10;
    GPIO_PORTB_AFSEL_R |= 0x10;
    GPIO_PORTB_DEN_R &= ~0x10;
    GPIO_PORTB_AMSEL_R |= 0x10;
    while(SYSCTL_PRADC_R & 0x01 == 0){}; //wait for system control to flag adc0 peripheral is ready  SAMPLE EDIT FROM NOTHING TO THIS WAIT

    ADC0_PC_R &= ~0xF;
    ADC0_PC_R |= 0x1;
    ADC0_SSPRI_R = 0x3210; //sequence select 0 highest priority
    ADC0_ACTSS_R = 0x01;
    ADC0_EMUX_R &= ~0xF; // SAMPLE EDIT FROM NORMAL TO ~
    ADC0_SSMUX0_R &= 0xFFF0; //zero's select the gpio in the mux
    ADC0_SSMUX0_R += 10; //sets Temp sensor and end of sequence 0b1010
    ADC0_SSCTL0_R = 0x6; //SAMPLE EDIT FROM A TO 6
//    ADC0_IM_R &= 0xFFFE; //disable interrupt on SS0
    ADC0_ACTSS_R |= 0x01;
    ADC0_SAC_R |= 0x4;
}

uint16_t adc_read(void){
    ADC0_PSSI_R |= 0x01;
    while((ADC0_RIS_R & 0x01) == 0){}; //wait for scan complete interrupt flag
    uint16_t t = ADC0_SSFIFO0_R & 0x00000FFF;  //Grab last 12 bits
    ADC0_ISC_R = 0x01; //Clear interrupt
    return t;
}


