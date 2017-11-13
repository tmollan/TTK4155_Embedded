
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
	
	
	filter *ball = malloc(sizeof(filter));
	if (ball == NULL) printf("ball malloc failed\n");
	
	gameFlags *prevFlags = malloc(sizeof(gameFlags));
	if (prevFlags == NULL) printf("prevFlag malloc failed\n");

    while (1) {
		prevFlags->byte = game->flags.byte;
		getGameInfo(game);

		if (game->flags.mode == GAME_ON) {
			
			if (game->flags.difficulty == GDIFF_HARD) {
				setModePID(FEEDBACK, PID);
			} else {
				setModePID(OPEN_LOOP, PID);
			}
			
			// If compare match flag for PID sampling timer is set
			if (testBit(TIFR3, OCF3A)) {
				setBit(TIFR3, OCF3A);			// Clear flag
				runMotor(game->joyPos, PID);
			}
			
			driveServo(-(game->sliderPos*2-100));

			if (game->flags.jButtonPressed && !prevFlags->jButtonPressed)
				triggerSolenoid();

			if (ballDetected(ball)) {
				if (game->lives > 0) {
					game->lives--;
					sendGameInfo(game);
				}
			}

		} else {
			driveServo(0);		// Center servo
			writeTWI(0);		// Motor zero speed
			
			if (game->flags.calibrate && !prevFlags->calibrate) {
				calibrateEncoderMax();
			}
		}

    }

}
