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
// Flags for game status
typedef union {
    uint8_t byte;
    struct {
        uint8_t loaded : 1,             // Game is loaded
				mode : 1,               // Game mode
                difficulty : 1,         // Game difficulty
                newGame : 1,            // A new game is started
				calibrate : 1,          // Calibrate is initiated
				lButtonPressed : 1,     // Left button is pressed
				rButtonPressed : 1,     // Right button is pressed
				jButtonPressed : 1;     // Joy. button is pressed
    };
} gameFlags;

// Struct for game status
typedef struct {
    gameFlags flags;
    int8_t joyPos;      // x-position
    uint8_t lives;      // Remaining lives
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
