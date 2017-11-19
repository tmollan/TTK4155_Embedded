/*

CAN.h

*/

#ifndef CAN_H_
#define CAN_H_

// Includes
#include <stdint.h>
#include <stdio.h>
#include "CANctrl.h"

// Defines
// CAN frame IDs
#define NO_MESSAGE 0x0000
#define GAME_MESSAGE 0x0001

// Struct for a CAN frame
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

// Flags for checking CAN buffer availabilities
typedef union {
	uint8_t byte;
	struct {
		uint8_t rb0_full : 1,
				rb1_full : 1,
				tb0_busy : 1,
				tb0_done : 1,
				tb1_busy : 1,
				tb1_done : 1,
				tb2_busy : 1,
				tb2_done : 1;
	};
} statusFlags;


// Functions
void initCAN(uint8_t mode);
void transmitCAN(CANmessage data);
CANmessage receiveCAN(void);
uint8_t getTransmitBufferCAN(void);
uint8_t getReceiveBufferCAN(void);
uint8_t transmitCompleteCAN(uint8_t buffer);


#endif /* CAN_H_ */
