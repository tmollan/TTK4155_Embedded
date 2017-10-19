
// Includes
#include "CAN.h"


//
void initCAN(void) {
	resetCAN();
	
	// Turns off receiving mask/filters, receives any message
	modifyBitCAN(RXB0CTRL, RXM0_MASK, 0x60);
	// Enable interrupt on received CAN messages
	modifyBitCAN(CANINTE, CANINTE_MASK, RX0IE);
	// Set in loopback mode
	modifyBitCAN(CANCTRL, MODE_MASK, MODE_LOOPBACK);
}


//
void transmitCAN(CANmessage message) {
	printf("before: %2x\n", message.id);
	message.id = message.id << 5;				// Bit shift
	printf("after: %2x\n", message.id);
	//printf("tra idBytes[0]: %2x\n", message.idBytes[0]);
	//printf("tra idBytes[1]: %2x\n", message.idBytes[1]);
	writeCAN(TXB0SIDL, message.idBytes[0]);
	writeCAN(TXB0SIDH, message.idBytes[1]);
	writeCAN(TXB0DLC, message.length);
	for (uint8_t byte = 0; byte < message.length; byte++) {
		writeCAN(TXB0D + byte, message.dataBytes[byte]);
	}
	sendRequestCAN(TBUFF0);
}


//
CANmessage receiveCAN(void) {
  CANmessage message;
  message.idBytes[0] = readCAN(RXB0SIDL);
  message.idBytes[1] = readCAN(RXB0SIDH);
  printf("rec idBytes[0]: %2x\n", message.idBytes[0]);
  printf("rec idBytes[1]: %2x\n", message.idBytes[1]);
  printf("before: %2x\n", message.id);
  message.id = message.id >> 5;				// Bit shift
  printf("after: %2x\n", message.id);
  message.length = readCAN(RXB0DLC);
  for (uint8_t byte = 0; byte < message.length; byte++) {
	  message.dataBytes[byte] = readCAN(RXB0D + byte);
  }
  return message;
}
