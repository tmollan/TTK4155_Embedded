/*

CAN.h

*/


#ifndef CAN_H_
#define CAN_H_

#include <stdint.h>
#include <stdio.h>
#include "CANctrl.h"

#define CANCTRL_REG_ADDR 0x0F
#define OP_MODE_MASK 0xE0
#define LOOPBACK_MODE 0x40

#define CANINTE_REG_ADDR 0x2B	// CAN interrupt register address
#define CANINTE_TRANS_MASK 0xF1	// Mask for transmit enabling interrupts

// Receive buffer 0
#define RXB0CTRL 0x60			// Control register
#define RXM0MASK 0x60			// Mask for receive mask/filters
#define RXM0HIGH 0x60			// Turns mask/filters off, receives any message
#define RXB0SIDH 0x61			// standard identifier register high
#define RXB0SIDL 0x62			// standard identifier register low
#define RXB0DLC 0x65			// data length code register
#define RXB0D 0x66				// data byte register (0x66-6D)

// Transmit buffer 0
#define TX0IE 0x04				// empty interrupt enable bit
#define TXB0CTRL 0x30			// control register
#define TXB0SIDH 0x31			// standard identifier register high
#define TXB0SIDL 0x32			// standard identifier register low
#define TXB0DLC 0x35			// data length code register
#define TXB0D 0x36				// data byte register (0x36-3D)


/*
#define LOOPBACK 0
typedef enum {
  LOOPBACK
} CANmode;
*/

typedef struct {
	uint8_t id;
	uint8_t length;
	union {
		uint8_t bytes[8];
		uint16_t position[4];
	};
} CANmessage;


void initCAN(void);
void transmitCAN(CANmessage data);
CANmessage receiveCAN(void);


#endif /* CAN_H_ */
