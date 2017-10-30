/*

CAN.h

*/

#ifndef CAN_H_
#define CAN_H_

// Includes
#include <stdint.h>
#include <stdio.h>
#include "CANctrl.h"


typedef union {
	uint8_t byte;
	struct {
		uint8_t on : 1,
				mode : 2,
				tb0_done : 1,
				tb1_busy : 1,
				tb1_done : 1,
				tb2_busy : 1,
				tb2_done : 1;
	};
} gameFlags;


void transmitGameStatus(CANmessage gameStatus);


#endif /* CAN_H_ */
