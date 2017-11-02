
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
#include "game.h"


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

	initCAN(MODE_NORMAL);

	gameInfo *game = malloc(sizeof(gameInfo));
	initGame(game);

	menupage *gameMenu = malloc(sizeof(menupage));
	initGameMenu(&gameMenu);


	while (1) {

		// If menu mode is active
		if (menuMode) {
			navigateMenu(&currentMenu);

		// Run application
		} else {
			switch (currentApp) {


				// Ping-pong game
				case APP_GAME:
					switch (game->flags.mode) {

						case GAME_OFF:
							if (!game->flags.loaded) loadGame(game, gameMenu);
							navigateMenu(&gameMenu);
							break;

						default:
							getGameInfo(game);
							updateGameScreen(game);
							if (game->lives == 0) endGame(game, gameMenu);
							sendGameInfo(game);
					}
					break;

				case APP_GAMEDIFF:
					setDifficulty(currentMenuIndex);
					currentApp = APP_GAME;
					loadMenu(currentMenuIndex, gameMenu);
					break;

				case APP_NEWGAME:
					startGame(game);
					currentApp = APP_GAME;
					drawStartScreen();
					break;

				case APP_EXITGAME:
					exitGame(game);
					exitApp(currentMenu);
					break;




				// Drawing on OLED with joystick
				case APP_DRAW:
					if (buttonPressed(JOYSTICK)) clearDisplaySRAM();
					drawJoystickSRAM();
					refreshOLED();
					_delay_ms(25);	// Speed of drawing
					break;



				// Joystick calibration procedure
				case APP_CALIBRATE:
					calibrateJoystick();
					drawCheckMark();
					exitApp(currentMenu);
					break;



				// Changes font on OLED
				case APP_FONTSIZE:
					setFont(currentMenuIndex);
					exitApp(currentMenu);
					break;


				default: printf("No active apps.\n");
			}
		}

		/*
		// For read- and write-test of SRAM
		uint16_t address = 0x000;			// 0x000-0x800 (2048 addresses)
		uint8_t writeData, readData;		// 0x00-0xFF (0-255)
		*/


		/*// CAN test
		CANmessage myMessage;
		myMessage.id = 0x0123;		// Max 0x07ef
		myMessage.length = 5;
		for (int i = 0, n = 1; i < myMessage.length; i++, n++) {
			myMessage.dataBytes[i] = n;
		}

		printf("Before transmit:\nID: %.4x\nlength: %d\n\n", myMessage.id, myMessage.length);
		transmitCAN(myMessage);
		_delay_ms(500);
		CANmessage recMessage = receiveCAN();
		printf("After transmit:\nID: %.4x\nlength: %d\n\n", recMessage.id, recMessage.length);

		for (int i = 0; i < recMessage.length; i++) {
			printf("%d", recMessage.dataBytes[i]);
		}*/

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
