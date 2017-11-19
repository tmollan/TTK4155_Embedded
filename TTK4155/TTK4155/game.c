
// Includes
#include "game.h"

// Globals
int8_t timerCount;
int16_t gameSeconds;

// Initializes game menu
void initGameMenu(menupage **menu) {
    // Creates menu pages
    menupage *gamePage = newMenu(NULL);
    menupage *settingsPage = newMenu(gamePage);

    // Creates game page menu items
    menuitem **gameItems = malloc(4 * sizeof(menuitem*));
    gameItems[0] = newItem("New game", NULL, APP_NEWGAME);
    gameItems[1] = newItem("Difficulty", settingsPage, APP_NONE);
	gameItems[2] = newItem("Calibrate", NULL, APP_ENCODERCAL);
    gameItems[3] = newItem("Exit", NULL, APP_EXITGAME);
    gamePage->items = malloc(sizeof(gameItems));
    gamePage->items = gameItems;
    gamePage->length = 4;

    // Creates settings page menu items
    menuitem **settingsItems = malloc(2 * sizeof(menuitem*));
    settingsItems[0] = newItem("Easy", NULL, APP_GAMEDIFF);
    settingsItems[1] = newItem("Hard", NULL, APP_GAMEDIFF);
    settingsPage->items = malloc(sizeof(settingsItems));
    settingsPage->items = settingsItems;
    settingsPage->length = 2;

    // Sets current menu to game page
    *menu = gamePage;

	// Game timer for score
	// Set clock prescaling to clk/1 (1: prescaler value)
	setBit(TCCR1B, CS10);

	// Reset timer
	TCNT1 = 0;
	timerCount = 0;
	gameSeconds = 0;

}

// Initializes game
void initGame(gameInfo *game) {
    game->flags.loaded = 0;
    game->flags.mode = GAME_OFF;
    game->flags.newGame = 0;
}

// Loads game and menu
void loadGame(gameInfo *game, menupage *menu) {
    game->flags.loaded = 1;
    drawStartScreen();
    currentMenuIndex = 0;
    loadMenu(currentMenuIndex, menu);
}

// Starts the game
void startGame(gameInfo *game) {
    game->flags.mode = GAME_ON;
    game->lives = 3;
	timerCount = 0;
	gameSeconds = 0;

    // Sends new game flag to node 2
    game->flags.newGame = 1;
	sendGameInfo(game);
    game->flags.newGame = 0;
}

// Ends the game
void endGame(gameInfo *game, menupage *menu) {
    // Print game over screen
    int8_t tempFont = currentFont;
	setFont(FONT_BIG);
	char *string = "GAME OVER";
	uint16_t address = posAddressSRAM(3, 63-strlen(string)*fontWidth/2);
    clearDisplaySRAM();
    drawStringSRAM(string, address);

	string = "SCORE:";
	address = posAddressSRAM(5, 63-strlen(string)*fontWidth/2);
	drawStringSRAM(string, address);
	char seconds[5];
	sprintf(seconds, "%d", gameSeconds);
	address = posAddressSRAM(6, 63-strlen(seconds)*fontWidth/2);
	drawStringSRAM(seconds, address);

    // Flash game over screen
    for (int8_t i = 0; i < 9; i++) {
    	refreshOLED();
        _delay_ms(500);
        invertDisplaySRAM();
    }
	setFont(tempFont);

    // Go back to game menu
    game->flags.mode = GAME_OFF;
    loadMenu(currentMenuIndex, menu);
}

// Exits the game menu
void exitGame(gameInfo *game) {
    game->flags.loaded = 0;
    currentMenuIndex = 0;
}

// Updates game screen while playing
void updateGameScreen(gameInfo *game) {
    int8_t tempFont = currentFont;
	setFont(FONT_BIG);

    // Increment timer count on timer overflows
	if (testBit(TIFR, TOV1)) {
		setBit(TIFR, TOV1);			// Clear flag
		timerCount++;
	}

    // Increment second count each second
	if (timerCount >= 75) {
		gameSeconds++;
		timerCount = 0;
	}

    // Print game screen with current lives and time played (score)
    clearDisplaySRAM();
	char *string = "LIVES:";
	uint16_t address = posAddressSRAM(2, 63-strlen(string)*fontWidth/2);
    drawStringSRAM(string, address);
    char lives[2];
    sprintf(lives, "%d", game->lives);
    address = posAddressSRAM(3, 63-strlen(lives)*fontWidth/2);
    drawStringSRAM(lives, address);

	string = "TIME:";
	address = posAddressSRAM(5, 63-strlen(string)*fontWidth/2);
	drawStringSRAM(string, address);
	char seconds[5];
	sprintf(seconds, "%d", gameSeconds);
	address = posAddressSRAM(6, 63-strlen(seconds)*fontWidth/2);
	drawStringSRAM(seconds, address);

	refreshOLED();

	setFont(tempFont);
}

// Checks and updates button states (active once per click)
void updateButtonStates(gameInfo *game) {
    if (!game->flags.lButtonPressed && buttonPressed(LEFTBUTTON)) {
        game->flags.lButtonPressed = 1;
    } else if (game->flags.lButtonPressed && !buttonPressed(LEFTBUTTON)) {
        game->flags.lButtonPressed = 0;
    }
    if (!game->flags.rButtonPressed && buttonPressed(RIGHTBUTTON)) {
        game->flags.rButtonPressed = 1;
    } else if (game->flags.rButtonPressed && !buttonPressed(RIGHTBUTTON)) {
        game->flags.rButtonPressed = 0;
    }
    if (!game->flags.jButtonPressed && buttonPressed(JOYSTICK)) {
        game->flags.jButtonPressed = 1;
    } else if (game->flags.jButtonPressed && !buttonPressed(JOYSTICK)) {
        game->flags.jButtonPressed = 0;
    }
}

// Sends game status as CAN frame to node 2
void sendGameInfo(gameInfo *game) {
    joystick myJoystick = getJoystick();
	sliders mySliders = getSliders();

    CANmessage msg;
    msg.id = GAME_MESSAGE;		// Max 0x07ef
    msg.length = 4;
    msg.dataBytes[0] = game->flags.byte;
    msg.dataBytes[1] = myJoystick.xPos;
	msg.dataBytes[2] = game->lives;
	msg.dataBytes[3] = mySliders.rightPos;

    transmitCAN(msg);
}

// Updates game status from CAN frame from node 2
void getGameInfo(gameInfo *game) {
    CANmessage msg = receiveCAN();
    if (msg.id == GAME_MESSAGE) {
        game->lives = msg.dataBytes[2];
    }
}

// Sets the game difficulty flag
void setDifficulty(gameInfo *game, int8_t diff) {
    game->flags.difficulty = diff;
}
