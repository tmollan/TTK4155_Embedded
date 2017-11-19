
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

	initUART(MYUBRR);		// Computer comm.
	initSRAM();				// Extern SRAM
	//testSRAM();				// Verifies that SRAM is working properly
	initJoystick();			// Initializes (and calibrates) joystick
	initOLED();
	initCAN(MODE_NORMAL);	// Node 2 CAN bus comm.

	menupage *currentMenu = malloc(sizeof(menupage));
	initMenu(&currentMenu);

	gameInfo *game = malloc(sizeof(gameInfo));
	initGame(game);

	menupage *gameMenu = malloc(sizeof(menupage));
	initGameMenu(&gameMenu);


	// Main loop
	while (1) {

		// If menu mode is active
		if (menuMode) {
			navigateMenu(&currentMenu);


		// If not in menu mode
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
							updateButtonStates(game);

							if (buttonPressed(LEFTBUTTON)) {
								game->flags.mode = GAME_OFF;
								loadMenu(currentMenuIndex, gameMenu);
							} else if (game->lives == 0) {
								endGame(game, gameMenu);
							}

							sendGameInfo(game);
					}
					break;

				// Calibrates motor encoder and returns to game menu
				case APP_ENCODERCAL:
					// Send calibrate flag once
					game->flags.calibrate = 1;
					sendGameInfo(game);
					game->flags.calibrate = 0;

					gotoGameApp();
					loadMenu(currentMenuIndex, gameMenu);
					break;

				// Sets game difficulty and returns to game menu
				case APP_GAMEDIFF:
					setDifficulty(game, currentMenuIndex);
					drawCheckMark();
					gotoGameApp();
					loadMenu(currentMenuIndex, gameMenu);
					break;

				// Starts a new game
				case APP_NEWGAME:
					startGame(game);
					gotoGameApp();
					break;

				// Exits game menu and enters main menu mode
				case APP_EXITGAME:
					exitGame(game);
					exitApp(currentMenu);
					break;




				// Drawing on OLED with joystick
				case APP_DRAW:
					// Clear display on button press
					if (buttonPressed(JOYSTICK)) clearDisplaySRAM();

					drawJoystickSRAM();
					refreshOLED();
					_delay_ms(25);	// Speed of drawing

					// Return to menu on button press
					if (buttonPressed(LEFTBUTTON)) exitApp(currentMenu);
					break;



				// Initiate calibrate joystick procedure and return to menu
				case APP_CALIBRATE:
					calibrateJoystick();
					drawCheckMark();
					exitApp(currentMenu);
					break;



				// Sets a new font and returns to menu
				case APP_FONTSIZE:
					setFont(currentMenuIndex);
					exitApp(currentMenu);
					break;


				// If there is no current application running
				default: printf("No active apps. %d\n", (int8_t)currentApp);
			}
		}
	}
}
