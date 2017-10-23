/*
 * node2.c
 *
 * Created: 23.10.2017 10:48:47
 *  Author: torkilsm
 */ 

#include "setup.h"
#include <avr/io.h>
#include "driverUART.h"
#include "SPI.h"
#include "CANctrl.h"
#include "CAN.h"


int main(void)
{
	// For printf() functionality
	stdout = &mystdout;		// Redirect stdout to UART
	stdin = &mystdin;		// Redirect stdin to UART
	
	initUART(MYUBRR);
	
	initCAN(MODE_NORMAL);
	
    while(1)
    {
		CANmessage msg = receiveCAN();
		if (msg.id != NO_MESSAGE) {
			printf("Id: 0x%.4x\n", msg.id);
			printf("Length: %d\n", msg.length);
			printf("Joy.x: %d\n", (int8_t)msg.dataBytes[0]);
			printf("Joy.y: %d\n", (int8_t)msg.dataBytes[1]);
			printf("Joy.angle: %d\n", (uint16_t)(msg.dataBytes[2] << 8) + msg.dataBytes[3]);
			printf("Joy.dir: %d\n", msg.dataBytes[4]);
			printf("Left slider: %d\n", msg.dataBytes[5]);
			printf("Right slider: %d\n", msg.dataBytes[6]);
			printf("\n\n");
		}
		_delay_ms(1000);
		
    }
}