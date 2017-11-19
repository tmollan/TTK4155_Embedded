
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
	_delay_ms(1);
}


uint8_t getModeCAN(void) {
	return readCAN(CANSTAT) & MODE_MASK;
}

void setModeCAN(uint8_t mode) {
	modifyBitCAN(CANCTRL, MODE_MASK, mode);
}

// Check if CAN is in mode
uint8_t inModeCAN(uint8_t mode) {
	uint8_t currentMode = getModeCAN();
	if (currentMode != mode) {
		/* For troubleshooting
		switch (mode) {
			case MODE_NORMAL:
				printf("CAN is NOT in Normal Operation mode: CANSTAT = 0x%.2x\n", currentMode);
				break;
			case MODE_SLEEP:
				printf("CAN is NOT in Sleep mode: CANSTAT = 0x%.2x\n", currentMode);
				break;
			case MODE_LOOPBACK:
				printf("CAN is NOT in Loopback mode: CANSTAT = 0x%.2x\n", currentMode);
				break;
			case MODE_LISTENONLY:
				printf("CAN is NOT in Listen-only mode: CANSTAT = 0x%.2x\n", currentMode);
				break;
			case MODE_CONFIG:
				printf("CAN is NOT in Configuration mode: CANSTAT = 0x%.2x\n", currentMode);
				break;
			case MODE_POWERUP:
				printf("CAN is NOT in Power-up mode: CANSTAT = 0x%.2x\n", currentMode);
				break;
		}*/
		return 0;
	}
	return 1;
}
