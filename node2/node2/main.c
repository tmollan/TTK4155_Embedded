
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

	initUART(MYUBRR);		// Computer comm.
	initCAN(MODE_NORMAL);	// Node 1 CAN bus comm.

	initPWM();		// PWM for servo control
	initADC();		// IR sensor readings

	// Motor and PID controller
	PIDcontroller *PID = malloc(sizeof(PIDcontroller));
	initPID(P_GAIN, I_TIME, D_TIME, S_TIME, PID);
	initMotor();

	gameInfo *game = malloc(sizeof(gameInfo));
	initGame(game);

	// For ball detection
	filter *ball = malloc(sizeof(filter));

	// For comparing current and previous flags
	gameFlags *prevFlags = malloc(sizeof(gameFlags));


	// Main loop
    while (1) {

		// Remember previous status flags
		prevFlags->byte = game->flags.byte;

		getGameInfo(game);		// Get game info from node 1


		// If game is ON
		if (game->flags.mode == GAME_ON) {

			// Set game difficulty
			if (game->flags.difficulty == GDIFF_HARD) {
				setModePID(FEEDBACK, PID);		// Hard
			} else {
				setModePID(OPEN_LOOP, PID);		// Easy
			}

			// If timer compare match flag for PID sampling is set
			if (testBit(TIFR3, OCF3A)) {
				setBit(TIFR3, OCF3A);			// Clear flag
				runMotor(game->joyPos, PID);
			}

			// Right slider drives servo
			driveServo(-(game->sliderPos*2-100));	// 0-100 -> -100-100

			// Trigger solenoid once per joystick button click
			if (game->flags.jButtonPressed && !prevFlags->jButtonPressed)
				triggerSolenoid();

			// Every time a ball is detected
			if (ballDetected(ball)) {
				// Decrease lives and send status to node 1
				if (game->lives > 0) {
					game->lives--;
					sendGameInfo(game);
				}
			}


		// If game is OFF
		} else {
			driveServo(0);		// Center servo
			writeTWI(0);		// Zero motor speed

			// Calibrate flag gets set from menu on node 1
			if (game->flags.calibrate && !prevFlags->calibrate) {
				calibrateEncoderMax();
			}
		}

    }

}
