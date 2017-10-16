
#define SSPIN_SPI PINB4

// SPI instructions
#define WRITE_INST 0x02
#define READ_INST 0x03
#define READSTATUS_INST 0xA0
#define RTS_INST 0x80
#define BITMOD_INST 0x05
#define RESET_INST 0xC0

// Includes
#include "CANctrl.h"


// Writes data to CAN controller
void writeCAN(uint8_t address, uint8_t data) {
	clearBit(PORTB, SSPIN_SPI);		// Slave Select: SPI
	writeSPI(WRITE_INST);			// Write instruction
	writeSPI(address);				// Which address to write to
	writeSPI(data);					// Writes data to address
	setBit(PORTB, SSPIN_SPI);		// Release slave
}


// Reads data from CAN controller
uint8_t readCAN(uint8_t address) {
	clearBit(PORTB, SSPIN_SPI);		// Slave Select: SPI
	writeSPI(READ_INST);			// Read instruction
	writeSPI(address);				// Which address to read
	uint8_t data = readSPI();		// Reads data from address
	setBit(PORTB, SSPIN_SPI);		// Release slave
	return data;
}


// Allows single instruction access to status bits
uint8_t readStatusCAN(void) {
	clearBit(PORTB, SSPIN_SPI);		// Slave Select: SPI
	writeSPI(READSTATUS_INST);		// Read status instruction
	uint8_t status = readSPI();		// Reads status
	setBit(PORTB, SSPIN_SPI);		// Release slave
	return status;
}


// Used to initiate message transmission for one or more transmit buffers
void sendRequestCAN(uint8_t buffers) {
	clearBit(PORTB, SSPIN_SPI);		// Slave Select: SPI
	writeSPI(RTS_INST + buffers);	// Writes which transmit buffers to be enabled
	setBit(PORTB, SSPIN_SPI);		// Release slave
}


// Used to set or clear individual bits in status and control registers
void modifyBitCAN(uint8_t address, uint8_t mask, uint8_t data) {
	clearBit(PORTB, SSPIN_SPI);		// Slave Select: SPI
	writeSPI(BITMOD_INST);			// Bit modify instruction 
	writeSPI(address);				// Which address to write to
	writeSPI(mask);					// Which bits in register allowed to change
	writeSPI(data);					// Which value the bits in register will change to
	setBit(PORTB, SSPIN_SPI);		// Release slave
}


// Used to re-initialize internal registers and set config. mode
// Same functionality as reset pin
void resetCAN(void) {
	clearBit(PORTB, SSPIN_SPI);		// Slave Select: SPI
	writeSPI(RESET_INST);			// Reset instruction
	setBit(PORTB, SSPIN_SPI);		// Release slave
	_delay_ms(10);
}
