
// Includes
#include "SPI.h"

// Initializes SPI
void initSPI(void) {
	makeOutput(DDR_SPI, SSPIN);
	makeOutput(DDR_SPI, MOSIPIN);
	makeInput(DDR_SPI, MISOPIN);
	makeOutput(DDR_SPI, SCKPIN);
	makeOutput(DDR_SPI, PINB0);

	setBit(SPCR, SPR0);		// Clock rate fck/16
	setBit(SPCR, MSTR);		// Enable master
	setBit(SPCR, SPE);		// Enable SPI

	releaseSlave();		// No selected slaves
	printf("SPI enabled\n");
}


// Sends data to and receives data from selected SPI slave
uint8_t transmitSPI(uint8_t data) {
	SPDR = data;				// Put data in transmission buffer
	loopUntilSet(SPSR, SPIF);	// Wait for transmission complete
	return SPDR;				// Received data is in transmission buffer
}


void selectSlave(void) {
	clearBit(PORT_SPI, SSPIN);
}

void releaseSlave(void) {
	setBit(PORT_SPI, SSPIN);
}
