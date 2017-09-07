/*
 * driverUART.h
 *
 * Created: 07.09.2017 10:56:25
 *  Author: torkilsm
 */ 


#ifndef DRIVERUART_H_
#define DRIVERUART_H_

#define CLKSPEED 4915200 // Clock Speed
#define BAUD 9600
#define MYUBRR CLKSPEED/16/BAUD-1

void USART_Init(unsigned int ubrr);
void USART_Transmit(unsigned char data, FILE *stream);
unsigned char USART_Receive(void);
void USART_Flush(void);

FILE mystdout = FDEV_SETUP_STREAM(USART_Transmit, NULL, _FDEV_SETUP_WRITE);



#endif /* DRIVERUART_H_ */