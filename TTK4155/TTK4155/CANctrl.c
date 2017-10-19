
// Includes
#include "CANctrl.h"


// Writes data to CAN controller
void writeCAN(uint8_t address, uint8_t data) {
	selectSlave();
	transmitSPI(WRITE_INST);		// Write instruction
	transmitSPI(address);			// Which address to write to
	transmitSPI(data);				// Writes data to address
	releaseSlave();
}


// Reads data from CAN controller
uint8_t readCAN(uint8_t address) {
	selectSlave();
	transmitSPI(READ_INST);				// Read instruction
	transmitSPI(address);				// Which address to read
	uint8_t data = transmitSPI(DUMMY);	// Reads data from address
	releaseSlave();
	return data;
}


// Allows single instruction access to status bits
uint8_t readStatusCAN(void) {
	selectSlave();
	transmitSPI(READSTATUS_INST);			// Read status instruction
	uint8_t status = transmitSPI(DUMMY);	// Reads status
	releaseSlave();
	return status;
}


// Used to initiate message transmission for one or more transmit buffers
void sendRequestCAN(uint8_t buffers) {
	selectSlave();
	transmitSPI(RTS_INST | buffers);	// Requests a buffer to start transmitting
	releaseSlave();
}


// Used to set or clear individual bits in status and control registers
void modifyBitCAN(uint8_t address, uint8_t mask, uint8_t data) {
	selectSlave();
	transmitSPI(BITMOD_INST);		// Bit modify instruction
	transmitSPI(address);			// Which address to write to
	transmitSPI(mask);				// Which bits in register allowed to change
	transmitSPI(data);				// Which value the bits in register will change to
	releaseSlave();
}


// Used to re-initialize internal registers and set config. mode
// Same functionality as reset pin
void resetCAN(void) {
	selectSlave();
	transmitSPI(RESET_INST);		// Reset instruction
	releaseSlave();
	//_delay_ms(10);
}


// Self test
/*
uint8_t selfTestCAN(void) {
	uint8_t error = 0;
	uint8_t status = readCAN(CANSTAT);
	if () {
		printf("CAN controller is NOT in config. mode after reset!\n");
	}
	if ((value & MODE_MASK) != MODE_CONFIG) {
		UART_print("MCP2515 is NOT in configuration mode after reset!");
		return 1;
	}

	return error;
}*/
