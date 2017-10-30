
// Includes
#include "setup.h"
#include <avr/io.h>
#include <stdint.h>

// Headers
#include "driverUART.h"
#include "SPI.h"
#include "CANctrl.h"
#include "CAN.h"
#include "PWM.h"
#include "ADC.h"


int main(void) {
	// For printf() functionality
	stdout = &mystdout;		// Redirect stdout to UART
	stdin = &mystdin;		// Redirect stdin to UART

	initUART(MYUBRR);

	initCAN(MODE_NORMAL);
	
	initPWN();
	
	initADC();
	
	int8_t a = 0;
	int16_t ballCount = 0;
    while(1) {
		CANmessage msg = receiveCAN();
		if (msg.id != NO_MESSAGE) {
			/*printf("Id: 0x%.4x\n", msg.id);
			printf("Length: %d\n", msg.length);*/
			//printf("Joy.x: %d\n", (int8_t)msg.dataBytes[0]);
			a = (int8_t)msg.dataBytes[0];
			/*printf("Joy.y: %d\n", (int8_t)msg.dataBytes[1]);
			printf("Joy.angle: %d\n", (uint16_t)(msg.dataBytes[2] << 8) + msg.dataBytes[3]);
			printf("Joy.dir: %d\n", msg.dataBytes[4]);
			printf("Left slider: %d\n", msg.dataBytes[5]);
			printf("Right slider: %d\n", msg.dataBytes[6]);
			printf("\n\n");*/
		}
		driveServo(a);
		if (ballDetected()) ballCount++;
		printf("Balls detected: %d\n", ballCount);
    }
}
