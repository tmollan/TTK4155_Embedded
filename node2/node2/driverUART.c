
// Includes
#include "setup.h"		// For setBit(), loopUntilSet(), etc..
#include <avr/io.h>		// For use of defined AVR registers and bits
#include <stdio.h>		// For FILE variable and printf()


// Initializes UART interface
void initUART(unsigned int ubrr) {
	// Sets baud rate
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;

	// Enable receiver and transmitter
	setBit(UCSR0B, RXEN0);		// Receiver
	setBit(UCSR0B, TXEN0);		// Transmitter

	// Set frame format: 8data, 2stop bit
	//UCSR0C = (1 << URSEL0) | (1 << USBS0) | (3 << UCSZ00);

	printf("Initializing...\nUART communication running\n");
}


// Transmits data to UART by writing to UART Data Register (UDR0)
void transmitUART(unsigned char data, FILE *stream) {
	// For printf(): force adds a carriage return after newline
	if (data == '\n') {
		transmitUART('\r', stream);
	}
	loopUntilSet(UCSR0A, UDRE0);		// Wait until register is empty
	UDR0 = data;						// Put new data into register
}


// Receives data from UART by reading UART Data Register (UDR0)
unsigned char receiveUART(void/* FILE *stream */) {
	loopUntilSet(UCSR0A, RXC0);			// Wait for data to be received
	return UDR0;						// Return received data from register
}