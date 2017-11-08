
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
#include "TWI.h"
#include "motor.h"
#include "PID.h"
#include "game.h"

// Defines
// PID parameters
#define P_GAIN 1		// Proportional gain
#define I_TIME 1000		// Integral time 	[s]
#define D_TIME 0		// Derivative time 	[s]
#define S_TIME 10		// Sample time 		[ms]


int main(void) {
	// For printf() functionality
	stdout = &mystdout;		// Redirect stdout to UART
	stdin = &mystdin;		// Redirect stdin to UART

	initUART(MYUBRR);

	initCAN(MODE_NORMAL);

	initPWM();

	initADC();

	PIDcontroller *PID = malloc(sizeof(PIDcontroller));
	if (PID == NULL) printf("PID malloc failed\n");
	initPID(P_GAIN, I_TIME, D_TIME, S_TIME, PID);

	initMotor();

	gameInfo *game = malloc(sizeof(gameInfo));
	if (game == NULL) printf("game malloc failed\n");
	initGame(game);



	int8_t ballPresent = 0;
	int16_t ballCount = 0;
	gameFlags prevFlags;

    while (1) {
		prevFlags.byte = game->flags.byte;
		getGameInfo(game);

		setControllerMode(OPEN_LOOP, PID);
		runMotor(game->joyPos, PID);
		printf("Encoder: %d\n", readEncoder());

		if (game->flags.mode == GAME_ON) {
			driveServo(game->joyPos);

			// If compare match flag for PID sampling timer is set
			if (testBit(TIFR3, OCF3A)) {
				setBit(TIFR3, OCF3A);			// Clear flag
				runMotor(game->joyPos, PID);
			}

			if (game->flags.lButtonPressed && !prevFlags.lButtonPressed)
				triggerSolenoid();

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
