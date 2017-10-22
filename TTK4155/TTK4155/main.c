
// Includes
#include "setup.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

// Header files
#include "driverUART.h"
#include "SRAM.h"
#include "ADC.h"
#include "joystick.h"
#include "EEPROM.h"
#include "OLED.h"
#include "menu.h"
#include "SPI.h"
#include "CANctrl.h"
#include "CAN.h"


volatile int8_t menuMode, currentMenuIndex;
volatile enum application currentApp;

int main(void) {
	// For printf() functionality
	stdout = &mystdout;		// Redirect stdout to UART
	stdin = &mystdin;		// Redirect stdin to UART

	printf("Initializing...\n");

	initUART(MYUBRR);		// Initializes UART communication

	initSRAM();				// Initializes extern SRAM
	//testSRAM();				// Verifies that SRAM IC is working properly

	initJoystick();			// Initialize (and calibrate) joystick

	initOLED();				// Initializes OLED

	menupage *currentMenu = malloc(sizeof(menupage));
	initMenu(&currentMenu);

	initCAN(MODE_LOOPBACK);

	/*
	// For read- and write-test of SRAM
	uint16_t address = 0x000;			// 0x000-0x800 (2048 addresses)
	uint8_t writeData, readData;		// 0x00-0xFF (0-255)
	*/

	/*
	joystick myJoystick;
	sliders mySliders;
	*/


	// CAN test
	CANmessage myMessage;
	myMessage.id = 0x0123;		// Max 0x07ef
	myMessage.length = 5;
	for (int i = 0, n = 1; i < myMessage.length; i++, n++) {
		myMessage.dataBytes[i] = n;
	}

	printf("Before transmit:\nID: %.4x\nlength: %d\n", myMessage.id, myMessage.length);
	transmitCAN(myMessage);
	CANmessage recMessage = receiveCAN();
	printf("After transmit:\nID: %.4x\nlength: %d\n", recMessage.id, recMessage.length);

	for (int i = 0; i < recMessage.length; i++) {
		printf("%d", recMessage.dataBytes[i]);
	}


	while(1) {

		if (menuMode) {
			navigateMenu(&currentMenu);

		} else {
			// Return to menu mode
			if (buttonPressed(LEFTBUTTON) && buttonPressed(RIGHTBUTTON)) {
				menuMode = 1;
				loadMenu(currentMenuIndex, currentMenu);
				continue;
			}

			// Run application
			switch (currentApp) {

				case APP_CALIBRATE:
					calibrateJoystick();
					drawCheckMark();
					menuMode = 1;
					loadMenu(currentMenuIndex, currentMenu);
					break;

				case APP_DRAW:
					if (buttonPressed(JOYSTICK)) clearDisplaySRAM();
					drawJoystickSRAM();
					refreshOLED();
					_delay_ms(25);	// Speed of drawing
					break;

				case APP_FONTSIZE:
					setFont(currentMenuIndex);
					menuMode = 1;
					loadMenu(currentMenuIndex, currentMenu);
					break;

				default:
					printf("No active apps.\n");
			}
		}


		/*
		// Return joystick and slider positions
		_delay_ms(1000);
		myJoystick = getJoystick();
		printf("xPos: %d, yPos: %d, angle: %d, dir: %d\n", myJoystick.xPos, myJoystick.yPos, myJoystick.angle, myJoystick.dir);

		mySliders = getSliders();
		printf("Left: %d, right: %d\n", mySliders.leftPos, mySliders.rightPos);
		*/

		/*
		// Text entered should return
		input = receiveUART();			// Remember to declare variable
		printf("%c", input);
		*/

		/*
		// Read- and write-test of SRAM
		writeData = receiveUART();			// Reads UART for data to save
		writeSRAM(address, writeData);		// Write to SRAM
		readData = readSRAM(address);		// Read from SRAM
		printf("%c", readData);				// Print received data
		*/

	}
}
