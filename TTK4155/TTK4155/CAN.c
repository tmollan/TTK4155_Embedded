
// Includes
#include "CAN.h"


//
void initCAN(void) {
	//resetCAN();

	// Turns off receiving mask/filters, receives any message
	modifyBitCAN(RXB0CTRL, RXM0MASK, RXM0HIGH);
	// Enable interrupt on received CAN messages
	modifyBitCAN(CANINTE, CANINTE_MASK, RX0IE);
	// Set in loopback mode
	modifyBitCAN(CANCTRL, MODE_MASK, MODE_LOOPBACK);

	writeCAN(TXB0SIDL, 0x00);	// Message ID(L) always low
}


//
void transmitCAN(CANmessage message) {
	message.id = message.id << 5;				// Bit shift
	writeCAN(TXB0SIDL, message.idBytes[0]);
	writeCAN(TXB0SIDH, message.idBytes[1]);
	writeCAN(TXB0DLC, message.length);
	for (uint8_t byte = 0; byte < message.length; byte++) {
		writeCAN(TXB0D + byte, message.bytes[byte]);
	}
	printf("ID: %d\nlength: %d\n", message.id, message.length);
	sendRequestCAN(TBUFF0);
}


//
CANmessage receiveCAN(void) {
  CANmessage message;
  message.idBytes[0] = readCAN(RXB0SIDL);
  message.idBytes[1] = readCAN(RXB0SIDH);
  message.id = message.id >> 5;				// Bit shift
  message.length = readCAN(RXB0DLC);
  for (uint8_t byte = 0; byte < message.length; byte++) {
	  message.bytes[byte] = readCAN(RXB0D + byte);
  }
  return message;
}
