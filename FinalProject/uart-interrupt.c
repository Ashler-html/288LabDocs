/*
*
*   uart-interrupt.c
*
*
*
*   @author
*   @date
*/

// The "???" placeholders should be the same as in your uart.c file.
// The "?????" placeholders are new in this file and must be replaced.

#include <inc/tm4c123gh6pm.h>
#include <stdint.h>
#include <stdbool.h>
#include "driverlib/interrupt.h"
#include "uart-interrupt.h"
#include "Movement.h"

// These variables are declared as examples for your use in the interrupt handler.
volatile char command_byte = '0'; // byte value for special character used as a command
volatile int command_flag = 0; // flag to tell the main program a special command was received
volatile char input;
void uart_interrupt_init(void){

  //enable clock to GPIO port B
  SYSCTL_RCGCGPIO_R |= 0x02;

  //enable clock to UART1
  SYSCTL_RCGCUART_R |= 0x02;

  //wait for GPIOB and UART1 peripherals to be ready
  while ((SYSCTL_PRGPIO_R & 0x02) == 0) {};
  while ((SYSCTL_PRUART_R & 0x02) == 0) {};

  //enable digital functionality on port B pins
  GPIO_PORTB_DEN_R |= 0x03;

  //enable alternate functions on port B pins
  GPIO_PORTB_AFSEL_R |= 0x03;

  //enable UART1 Rx and Tx on port B pins
  GPIO_PORTB_PCTL_R |= 0x11;

  //calculate baud rate
  uint16_t iBRD = 8; //use equations
  uint16_t fBRD = 44; //use equations

  //turn off UART1 while setting it up
  UART1_CTL_R &= 0x00;

  //set baud rate
  //note: to take effect, there must be a write to LCRH after these assignments
  UART1_IBRD_R = iBRD;
  UART1_FBRD_R = fBRD;

  //set frame, 8 data bits, 1 stop bit, no parity, no FIFO
  //note: this write to LCRH must be after the BRD assignments
  UART1_LCRH_R |= 0x60;

  //use system clock as source
  //note from the datasheet UARTCCC register description:
  //field is 0 (system clock) by default on reset
  //Good to be explicit in your code
  UART1_CC_R = 0x0;

  //////Enable interrupts

  //first clear RX interrupt flag (clear by writing 1 to ICR)
  UART1_ICR_R |= 0x10;

  //enable RX raw interrupts in interrupt mask register
  UART1_IM_R |= 0x10; //pg 925

  //NVIC setup: set priority of UART1 interrupt to 1 in bits 21-23
  NVIC_PRI1_R = (NVIC_PRI1_R & 0xFF0FFFFF) | 0x00200000;

  //NVIC setup: enable interrupt for UART1, IRQ #6, set bit 6
  NVIC_EN0_R |= 0x40;

  //tell CPU to use ISR handler for UART1 (see interrupt.h file)
  //from system header file: #define INT_UART1 22
  IntRegister(INT_UART1, UART1_Handler);

  //globally allow CPU to service interrupts (see interrupt.h file)
  IntMasterEnable();

  //re-enable UART1 and also enable RX, TX (three bits)
  //note from the datasheet UARTCTL register description:
  //RX and TX are enabled by default on reset
  //Good to be explicit in your code
  //Be careful to not clear RX and TX enable bits
  //(either preserve if already set or set them)
  UART1_CTL_R |= 0x301;

}

void uart_sendChar(char data){
    while((UART1_FR_R & 0x20) != 0){};
     UART1_DR_R = data;

}

char uart_receive(void){
    char rdata = 0;
    while(UART1_FR_R & 0x10){};
    rdata = (char) (UART1_DR_R & 0xFF);
        return rdata;
}

char uart_receive_nonblocking(void){
    char rdata = 0;
    if((UART1_FR_R & 0x10) == 0){
        rdata = (char) (UART1_DR_R & 0xFF);
        return rdata;
    };
    return 0;
}

void uart_sendStr(const char *data){
	//TODO for reference see lcd_puts from lcd.c file
}

// Interrupt handler for receive interrupts
void UART1_Handler(void)
{
    oi_t *sensor_data = oi_alloc();
    char byte_received;
    command_flag = 0;
    //check if handler called due to RX event
    if ((UART1_MIS_R & 0x10) != 0)
    {


        //read the byte received from UART1_DR_R and echo it back to PuTTY
        //ignore the error bits in UART1_DR_R
        uart_sendChar(UART1_DR_R);
        byte_received = UART1_DR_R;
        input = byte_received;


        //byte was received in the UART data register
        //clear the RX trigger flag (clear by writing 1 to ICR)
        UART1_ICR_R |= 0x10;

        //if byte received is a carriage return
        if (byte_received == '\r')
        {
            //send a newline character back to PuTTY
            uart_sendChar('\n');
        }
        else
        {
            command_byte = byte_received;
            if (byte_received == 's') //stop
            {
                command_flag = 1;
            }
        }
    }
}
