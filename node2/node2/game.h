/*

game.h

*/

#ifndef GAME_H_
#define GAME_H_

// Includes
#include <stdint.h>
#include "CAN.h"
#include "ADC.h"

// Defines
// Game modes
#define GAME_OFF 0
#define GAME_ON 1

// Game difficulities
#define GDIFF_EASY 0
#define GDIFF_MEDIUM 1
#define GDIFF_HARD 2


// Typedefs
typedef union {
    uint8_t byte;
    struct {
        uint8_t loaded : 1,
				mode : 1,
                difficulty : 2,
                newGame : 1,
				usused3 : 1,
				unused2 : 1,
				unused1 : 1;
    };
} gameFlags;

typedef struct {
    gameFlags flags;
    int8_t joyPos;      // x-position
    uint8_t lives;
} gameInfo;


// Functions
void initGame(gameInfo *game);
void sendGameInfo(gameInfo *game);
void getGameInfo(gameInfo *game);
int8_t ballDetected(void);


#endif /* GAME_H_ */
