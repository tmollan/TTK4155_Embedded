
// Includes
#include "SPI.h"



// Initializes SPI
void initSPI(void) {
	makeOutput(DDRB, PINB4);	// Set SS output
	makeOutput(DDRB, PINB5);	// Set MOSI output
	makeInput(DDRB, PINB6);		// Set MISO input
	makeOutput(DDRB, PINB7);	// Set SCK output
	
	setBit(SPCR, SPR0);			// Clock rate fck/16
	setBit(SPCR, MSTR);			// Enable master
	setBit(SPCR, SPE);			// Enable SPI
	
	setBit(PORTB, PINB4);
}


// Writes to a slave
void writeSPI(uint8_t data) {
	SPDR = data;				// SPDR is buffer to be sent
	loopUntilSet(SPSR, SPIF);	// Wait for transmission complete
}


// Reads from a slave
uint8_t readSPI(void) {
	SPDR = 0x00;
	loopUntilSet(SPSR, SPIF);	// Wait for reception complete
	return SPDR;				// Return SPDR buffer that holds received data
}
