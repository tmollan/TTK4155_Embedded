
// Includes
#include "game.h"

void initGame(gameInfo *game) {
    game->flags.loaded = 0;
    game->flags.mode = GAME_OFF;
    game->joyPos = 0;
    game->lives = 3;
}

void getGameInfo(gameInfo *game) {
    CANmessage msg = receiveCAN();
    if (msg.id == GAME_MESSAGE) {
        game->flags.byte = msg.dataBytes[0];
        game->joyPos = (int8_t)msg.dataBytes[1];
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

int8_t ballDetected(void) {
    if (readADC() < 50) return 1;
	return 0;
}
