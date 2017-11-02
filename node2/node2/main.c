
// Includes
#include "setup.h"
#include <avr/io.h>
#include <stdint.h>
#include <stdlib.h>

// Headers
#include "driverUART.h"
#include "SPI.h"
#include "CANctrl.h"
#include "CAN.h"
#include "PWM.h"
#include "ADC.h"

#include "game.h"


int main(void) {
	// For printf() functionality
	stdout = &mystdout;		// Redirect stdout to UART
	stdin = &mystdin;		// Redirect stdin to UART

	initUART(MYUBRR);

	initCAN(MODE_NORMAL);

	initPWN();

	initADC();

	gameInfo *game = malloc(sizeof(gameInfo));
	initGame(game);


	int8_t ballPresent = 0;
	int16_t ballCount = 0;

    while (1) {

		getGameInfo(game);
		

		if (game->flags.mode == GAME_ON) {
			driveServo(game->joyPos);

			if (!ballPresent && ballDetected()) {
				if (game->lives > 0) {
					game->lives--;
					sendGameInfo(game);
				}

				ballCount++;
				ballPresent = 1;
				printf("Balls detected: %d\n", ballCount);

			} else if (ballPresent && !ballDetected()) {
				ballPresent = 0;
			}

		}

    }

}
