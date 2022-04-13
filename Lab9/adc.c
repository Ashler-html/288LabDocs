#include <inc/tm4c123gh6pm.h>
#include <stdint.h>
#include "uart.h"
#include "adc.h"
#include "uart-interrupt.h"
#include "lcd.h"
#include "movement.h"
#include "Timer.h"

void adc_init(void){
    SYSCTL_RCGCADC_R |= 0b1;
    SYSCTL_RCGCGPIO_R |= 0b00010;
    while ((SYSCTL_PRGPIO_R & 0x2) == 0) {};
    GPIO_PORTB_DIR_R &= ~0x10;
    GPIO_PORTB_AFSEL_R |= 0x10;
    GPIO_PORTB_DEN_R &= ~0x10;
    GPIO_PORTB_AMSEL_R |= 0x10;

    ADC0_PC_R &= ~0xF;
    ADC0_PC_R |= 0x1; // 8) configure for 125K samples/sec
    ADC0_SSPRI_R = 0x0123; // 9) Sequencer 3 is highest priority
    ADC0_ACTSS_R &= ~0x0008; // 10) disable sample sequencer 3
    ADC0_EMUX_R &= ~0xF000;
    ADC0_SSMUX3_R &= ~0x000F;
    ADC0_SSMUX3_R += 10; // 12) set channel
    ADC0_SSCTL3_R = 0x0006; // 13) no TS0 D0, yes IE0 END0
    ADC0_IM_R &= ~0x0008; // 14) disable SS3 interrupts
    ADC0_ACTSS_R |= 0x0008;
}

uint16_t adc_read(void){
    uint16_t result;
    ADC0_PSSI_R = 0x0008;
    while((ADC0_RIS_R&0x08)==0){};
    result = ADC0_SSFIFO3_R&0xFFF;
    ADC0_ISC_R = 0x0008;
    return result;
}
