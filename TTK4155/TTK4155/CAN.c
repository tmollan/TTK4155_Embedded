
// Includes
#include "CAN.h"


// Initializes CAN
void initCAN(uint8_t mode) {
	initSPI();
	resetCAN();
	if (inModeCAN(MODE_CONFIG)) {
		// Turns off receiving mask/filters (receives any message)
		modifyBitCAN(RXB0CTRL, RXM_MASK, RXM_OFF);
		// Enables interrupt on received CAN messages
		modifyBitCAN(CANINTE, CANINTE_MASK, RX0IE);
	}
	setModeCAN(mode);
	if (inModeCAN(mode)) {
		printf("CAN bus running\n");
	}
}


// Transmits frame on CAN
void transmitCAN(CANmessage message) {
	uint8_t transmitBuffer = getTransmitBufferCAN();

	if (!transmitBuffer) return;	// All buffers busy (error implementation?)

	// Buffer register address offset
	uint8_t addr;
	switch (transmitBuffer) {
		case TBUFF1: addr = 0x10;
		case TBUFF2: addr = 0x20;
		default: addr = 0x00;
	}

	// Transmit frame
	message.id = message.id << 5;
	writeCAN(TXB0SIDH + addr, message.idBytes[1]);
	writeCAN(TXB0SIDL + addr, message.idBytes[0]);
	writeCAN(TXB0DLC + addr, message.length);
	for (uint8_t byte = 0; byte < message.length; byte++) {
		writeCAN(TXB0D + addr + byte, message.dataBytes[byte]);
	}

	// Request to initiate transmission
	sendRequestCAN(transmitBuffer);
}


// Receives frame on CAN
CANmessage receiveCAN(void) {
	CANmessage message;
	uint8_t receiveBuffer = getReceiveBufferCAN();

	// No buffers ready
	if (!receiveBuffer) {
		// error implementation?
		message.id = NO_MESSAGE;
		return message;
	}

	// Buffer register address offset
	uint8_t addr;
	switch (receiveBuffer) {
		case RBUFF1: addr = 0x10;
		default: addr = 0x00;
	}

	// Receive frame
	message.idBytes[1] = readCAN(RXB0SIDH + addr);
	message.idBytes[0] = readCAN(RXB0SIDL + addr);
	message.id = message.id >> 5;
	message.length = readCAN(RXB0DLC + addr) & DLC_MASK;
	for (uint8_t byte = 0; byte < message.length; byte++) {
		message.dataBytes[byte] = readCAN(RXB0D + addr + byte);
	}

	// Clear flag for full buffer
	modifyBitCAN(CANINTF, RBUFF_MASK, receiveBuffer);
	return message;
}

// Gets the highest priority empty transmit buffer
uint8_t getTransmitBufferCAN(void) {
	statusFlags flags;
	flags.byte = readStatusCAN();

	// Priorities: tb2 > tb1 > tb0
	if (!flags.tb2_busy) return TBUFF2;
	if (!flags.tb1_busy) return TBUFF1;
	if (!flags.tb0_busy) return TBUFF0;

	return 0;
}

// Gets the highest priority full receive buffer
uint8_t getReceiveBufferCAN(void) {
	statusFlags flags;
	flags.byte = readStatusCAN();

	// Priorities: rb0 > rb1
	if (flags.rb0_full) return RBUFF0;
	if (flags.rb1_full) return RBUFF1;

	return 0;
}

// Checks if buffer has completed transmission
uint8_t transmitCompleteCAN(uint8_t buffer) {
	statusFlags flags;
	flags.byte = readStatusCAN();

	switch (buffer) {
		case TBUFF0: return flags.tb0_done;
		case TBUFF1: return flags.tb1_done;
		case TBUFF2: return flags.tb2_done;
	}
	return 0;
}
