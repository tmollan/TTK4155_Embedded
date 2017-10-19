/*

CAN.h

*/

#ifndef CAN_H_
#define CAN_H_


// CAN includes
#include <stdint.h>
#include <stdio.h>
#include "CANctrl.h"


// CAN defines
#define CANSTAT 0x0E			// CAN status

#define CANCTRL 0x0F			// CAN control
#define MODE_MASK 0xE0			// For changing mode
#define MODE_NORMAL 0x00
#define MODE_SLEEP 0x20
#define MODE_LOOPBACK 0x40
#define MODE_LISTENONLY 0x60
#define MODE_CONFIG 0x80
#define MODE_POWERUP 0xE0

#define CANINTE 0x2B			// CAN interrupt register address
#define CANINTE_MASK 0xFF		// For enabling interrupts

// Receive buffer 0
#define RX0IE 0x01				// Full interrupt enable bit
#define RXB0CTRL 0x60			// Control register
#define RXM0_MASK 0x60			// Mask for receive mask/filters
#define RXM0_HIGH 0x60			// Turns mask/filters off, receives any message
#define RXB0SIDH 0x61			// Standard identifier register high
#define RXB0SIDL 0x62			// Standard identifier register low
#define RXB0EIDH 0x63			// Standard identifier register high
#define RXB0EIDL 0x64			// Standard identifier register low
#define RXB0DLC 0x65			// Data length code register
#define RXB0D 0x66				// Data byte register (0x66-6D)

// Transmit buffer 0
#define TX0IE 0x04				// Empty interrupt enable bit
#define TXB0CTRL 0x30			// Control register
#define TXB0SIDH 0x31			// Standard identifier register high
#define TXB0SIDL 0x32			// Standard identifier register low
#define TXB0DLC 0x35			// Data length code register
#define TXB0D 0x36				// Data byte register (0x36-3D)

// SPI buffers
#define TBUFF0 0x01


/*
#define LOOPBACK 0
typedef enum {
  LOOPBACK
} CANmode;
*/

typedef struct {
	union {
		uint8_t idBytes[2];
		uint16_t id;
	};
	uint8_t length;
	union {
		uint8_t dataBytes[8];
		uint16_t position[4];
	};
} CANmessage;


// CAN functions
void initCAN(void);
void transmitCAN(CANmessage data);
CANmessage receiveCAN(void);


#endif /* CAN_H_ */
