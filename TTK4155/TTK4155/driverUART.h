

#ifndef DRIVERUART_H_
#define DRIVERUART_H_

#include <stdio.h>    // Neccesary for FILE variable

// For UART communication
void UART_Init(unsigned int ubrr);
void UART_Transmit(unsigned char data, FILE *stream);
unsigned char UART_Receive(void);
//void UART_Flush(void);

// For printf() functionality
FILE mystdout = FDEV_SETUP_STREAM(UART_Transmit, NULL, _FDEV_SETUP_WRITE);


#endif /* DRIVERUART_H_ */
