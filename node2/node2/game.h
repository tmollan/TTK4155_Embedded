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
    int8_t joyPos;         // x-position
    uint8_t lives;         // Remaining lives
	uint8_t sliderPos;     // 0-100 %
} gameInfo;

// Struct for IR sensor filtering
typedef struct {
	int8_t detected;       // Detection
	int8_t count;          // Number of detections
	int8_t samples;        // Number of samples
	int16_t sampleSum;     // Sum of all sample values
} filter;


// Functions
void initGame(gameInfo *game);
void sendGameInfo(gameInfo *game);
void getGameInfo(gameInfo *game);
int8_t ballDetected(filter *ballFilter);


#endif /* GAME_H_ */
