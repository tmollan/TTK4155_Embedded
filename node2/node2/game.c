
// Includes
#include "game.h"

int8_t timerCount = 0;
void initGame(gameInfo *game) {
    game->flags.loaded = 0;
    game->flags.mode = GAME_OFF;
    game->joyPos = 0;
    game->lives = 3;
	
	// Timer for ball detection
	// Set clock prescaling to clk/1024 (1024: prescaler value)
	setBit(TCCR0B, CS00);
	setBit(TCCR0B, CS02);

	// Reset timer
	TCNT0 = 0;
}

void getGameInfo(gameInfo *game) {
    CANmessage msg = receiveCAN();
    if (msg.id == GAME_MESSAGE) {
        game->flags.byte = msg.dataBytes[0];
        game->joyPos = (int8_t)msg.dataBytes[1];
		game->sliderPos = msg.dataBytes[3];
        if (game->flags.newGame) {
            game->lives = msg.dataBytes[2];
            game->flags.newGame = 0;
        }
    }
}

void sendGameInfo(gameInfo *game) {
    CANmessage msg;
    msg.id = GAME_MESSAGE;
    msg.length = 3;
    msg.dataBytes[2] = game->lives;

    transmitCAN(msg);
}

int8_t ballDetected(filter *ballFilter) {
	// Simple filter
	ballFilter->sampleSum += readADC()*10;
	ballFilter->samples++;
	if (ballFilter->samples == 5) {
		printf("adc: %d\n", ballFilter->sampleSum / ballFilter->samples);
		if (ballFilter->sampleSum / ballFilter->samples < 500 && !ballFilter->detected) {
			ballFilter->detected = 1;
			ballFilter->sampleSum = 0;
			ballFilter->samples = 0;
			return 1;
		} else if (ballFilter->detected && ballFilter->sampleSum / ballFilter->samples >= 500) {
			if (testBit(TIFR0, TOV0)) {
				setBit(TIFR0, TOV0);		// Clear flag
				timerCount++;
			}
			if (timerCount >= 90) {
				ballFilter->detected = 0;
				timerCount = 0;
			}
		}
		ballFilter->sampleSum = 0;
		ballFilter->samples = 0;
	}
	return 0;
}
