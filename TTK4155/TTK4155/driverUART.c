
//#include "driverUART.h"
#include <avr/io.h>
#include <stdio.h>


void UART_Init(unsigned int ubrr) {
	// Sets baud rate
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;

	// Enable receiver and transmitter
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	//setBit(UCSR0B, RXEN0);		// Receiver (not tested)
	//setBit(UCSR0B, TXEN0);		// Transmitter (not tested)

	// Set frame format: 8data, 2stop bit
	//UCSR0C = (1 << URSEL0) | (1 << USBS0) | (3 << UCSZ00);
}


void UART_Transmit(unsigned char data, FILE *stream) {
	// Wait for empty transmit buffer
	while (!(UCSR0A & (1<<UDRE0)));
	//loopUntilSet(UCSR0A, UDRE0);	// (not tested)

	// For printf() functionality
	if (data == '\n')
		UART_Transmit('\r', stream);

	// Put data into buffer, sends the data
	UDR0 = data;
}


unsigned char UART_Receive(void) {
	// Wait for data to be received
	while (!(UCSR0A & (1<<RXC0)));
	//loopUntilSet(UCSR0A, RXC0);		// (not tested)

	// Get and return received data from buffer
	return UDR0;
}

/*
void UART_Flush(void) {
	unsigned char dummy;
	while (UCSR0A & (1 << RXC0))
	dummy = UDR0;
}*/
