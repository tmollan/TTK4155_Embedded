/*

GAME.h

*/

#ifndef GAME_H_
#define GAME_H_

// Includes
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "menu.h"
#include "CAN.h"
#include "joystick.h"
#include "OLED.h"

// Defines
// Game modes
#define GAME_OFF 0
#define GAME_ON 1

// Game difficulities
#define GDIFF_EASY 0
#define GDIFF_HARD 1

// Typedefs
typedef union {
    uint8_t byte;
    struct {
        uint8_t loaded : 1,
				mode : 1,
                difficulty : 1,
                newGame : 1,
				calibrate : 1,
				lButtonPressed : 1,
				rButtonPressed : 1,
				jButtonPressed : 1;
    };
} gameFlags;

typedef struct {
    gameFlags flags;
    int8_t joyPos;      // x-position
    uint8_t lives;
} gameInfo;

// Functions
void initGameMenu(menupage **menu);
void initGame(gameInfo *game);
void loadGame(gameInfo *game, menupage *menu);
void startGame(gameInfo *game);
void endGame(gameInfo *game, menupage *menu);
void exitGame(gameInfo *game);
void updateGameScreen(gameInfo *game);
void updateButtonStates(gameInfo *game);
void sendGameInfo(gameInfo *game);
void getGameInfo(gameInfo *game);
void setDifficulty(gameInfo *game, int8_t diff);


#endif /* GAME_H_ */
