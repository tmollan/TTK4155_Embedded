
// Includes
#include "game.h"

int8_t timerCount;
int16_t gameSeconds;
void initGameMenu(menupage **menu) {
    menupage *gamePage = newMenu(NULL);
    menupage *settingsPage = newMenu(gamePage);

    menuitem **gameItems = malloc(4 * sizeof(menuitem*));
    gameItems[0] = newItem("New game", NULL, APP_NEWGAME);
    gameItems[1] = newItem("Difficulty", settingsPage, APP_NONE);
	gameItems[2] = newItem("Calibrate", NULL, APP_ENCODERCAL);
    gameItems[3] = newItem("Exit", NULL, APP_EXITGAME);
    gamePage->items = malloc(sizeof(gameItems));
    gamePage->items = gameItems;
    gamePage->length = 4;

    menuitem **settingsItems = malloc(2 * sizeof(menuitem*));
    settingsItems[0] = newItem("Easy", NULL, APP_GAMEDIFF);
    settingsItems[1] = newItem("Hard", NULL, APP_GAMEDIFF);
    settingsPage->items = malloc(sizeof(settingsItems));
    settingsPage->items = settingsItems;
    settingsPage->length = 2;

    *menu = gamePage;
	
	// Game timer
	// Set clock prescaling to clk/1 (1: prescaler value)
	setBit(TCCR1B, CS10);

	// Reset timer
	TCNT1 = 0;
	timerCount = 0;
	gameSeconds = 0;
	
}

void initGame(gameInfo *game) {
    game->flags.loaded = 0;
    game->flags.mode = GAME_OFF;
    game->flags.newGame = 0;
}

void loadGame(gameInfo *game, menupage *menu) {
    game->flags.loaded = 1;
    drawStartScreen();
    currentMenuIndex = 0;
    loadMenu(currentMenuIndex, menu);
}

void startGame(gameInfo *game) {
    game->flags.mode = GAME_ON;
    game->lives = 3;
	timerCount = 0;
	gameSeconds = 0;

    game->flags.newGame = 1;
	sendGameInfo(game);
    game->flags.newGame = 0;
}

void endGame(gameInfo *game, menupage *menu) {
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
	
    for (int8_t i = 0; i < 9; i++) {
    	refreshOLED();
        _delay_ms(500);
        invertDisplaySRAM();
    }
	setFont(tempFont);

    game->flags.mode = GAME_OFF;
    loadMenu(currentMenuIndex, menu);
}

void exitGame(gameInfo *game) {
    game->flags.loaded = 0;
    currentMenuIndex = 0;
}

void updateGameScreen(gameInfo *game) {
    int8_t tempFont = currentFont;
	setFont(FONT_BIG);
	
	if (testBit(TIFR, TOV1)) {
		setBit(TIFR, TOV1);			// Clear flag
		timerCount++;
	}
	
	if (timerCount >= 75) {
		gameSeconds++;
		timerCount = 0;
	}

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

void getGameInfo(gameInfo *game) {
    CANmessage msg = receiveCAN();
    if (msg.id == GAME_MESSAGE) {
        game->lives = msg.dataBytes[2];
    }
}

void setDifficulty(gameInfo *game, int8_t diff) {
    game->flags.difficulty = diff;
}
