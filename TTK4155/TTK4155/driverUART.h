/*

driverUART.h

*/

#ifndef DRIVERUART_H_
#define DRIVERUART_H_

// Includes
#include "setup.h"		// For setBit(), loopUntilSet(), etc..
#include <avr/io.h>		// For use of defined AVR registers and bits
#include <stdio.h>		// For FILE variable and printf()

// Functions
// For UART communication
void initUART(unsigned int ubrr);                     // Initializes UART interface
void transmitUART(unsigned char data, FILE *stream);  // Transmits data to UART
unsigned char receiveUART(void/* FILE *stream */);    // Receives data from UART

// For printf() functionality
FILE mystdout = FDEV_SETUP_STREAM(transmitUART, NULL, _FDEV_SETUP_WRITE);
FILE mystdin = FDEV_SETUP_STREAM(NULL, receiveUART, _FDEV_SETUP_READ);


#endif /* DRIVERUART_H_ */
