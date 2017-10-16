
// Includes
#include "CAN.h"


//
void initCAN(void) {
	// Turns off receiving mask/filters, receives any message
	modifyBitCAN(RXB0CTRL, RXM0MASK, RXM0HIGH);
	// Enable interrupt
	modifyBitCAN(CANINTE_REG_ADDR, CANINTE_TRANS_MASK, TX0IE);
	// Set in loopback mode
	modifyBitCAN(CANCTRL_REG_ADDR, OP_MODE_MASK, LOOPBACK_MODE);
	
	writeCAN(TXB0SIDL, 0x00);	// Message ID(L) always low
}


//
void transmitCAN(CANmessage message) {
	writeCAN(TXB0SIDH, message.id);
	writeCAN(TXB0DLC, message.length);
	for (uint8_t addr = 0; addr < message.length; addr++) {
		writeCAN(TXB0D+addr, message.bytes[addr]);
	}
	printf("ID: %d\nlength: %d\n", message.id, message.length);
	sendRequestCAN(0x01);
}


//
CANmessage receiveCAN(void) {
  CANmessage message;
  message.id = readCAN(RXB0SIDH);
  message.length = readCAN(RXB0DLC);
  for (uint8_t addr = 0; addr < message.length; addr++) {
	  message.bytes[addr] = readCAN(RXB0D+addr);
  }
  return message;
}
