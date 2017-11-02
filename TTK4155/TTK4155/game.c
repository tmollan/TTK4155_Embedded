
// Includes
#include "game.h"

void initGameMenu(menupage **menu) {
    menupage *gamePage = newMenu(NULL);
    menupage *settingsPage = newMenu(gamePage);

    menuitem **gameItems = malloc(3 * sizeof(menuitem*));
    gameItems[0] = newItem("New game", NULL, APP_NEWGAME);
    gameItems[1] = newItem("Difficulty", settingsPage, APP_NONE);
    gameItems[2] = newItem("Exit", NULL, APP_EXITGAME);
    gamePage->items = malloc(sizeof(gameItems));
    gamePage->items = gameItems;
    gamePage->length = 3;

    menuitem **settingsItems = malloc(3 * sizeof(menuitem*));
    settingsItems[0] = newItem("Easy", NULL, APP_GAMEDIFF);
    settingsItems[1] = newItem("Medium", NULL, APP_GAMEDIFF);
    settingsItems[2] = newItem("Hard", NULL, APP_GAMEDIFF);
    settingsPage->items = malloc(sizeof(settingsItems));
    settingsPage->items = settingsItems;
    settingsPage->length = 3;

    *menu = gamePage;
}

void initGame(gameInfo *game) {
    game->flags.loaded = 0;
    game->flags.mode = GAME_OFF;
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
	sendGameInfo(game);
	_delay_ms(1000);
}

void endGame(gameInfo *game, menupage *menu) {
    int8_t tempFont = currentFont;
	setFont(FONT_BIG);
	char *string = "GAME OVER";
	uint16_t address = posAddressSRAM(3, 63-strlen(string)*fontWidth/2);
    clearDisplaySRAM();
    drawStringSRAM(string, address);
    for (int8_t i = 0; i < 5; i++) {
    	refreshOLED();
        _delay_ms(1000);
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

    clearDisplaySRAM();
	char *string = "LIVES:";
	uint16_t address = posAddressSRAM(3, 63-strlen(string)*fontWidth/2);
    drawStringSRAM(string, address);
    char lives[2];
    sprintf(lives, "%d", game->lives);
    address = posAddressSRAM(4, 63-strlen(lives)*fontWidth/2);
    drawStringSRAM(lives, address);
	refreshOLED();

	setFont(tempFont);
}

void sendGameInfo(gameInfo *game) {
    joystick myJoystick = getJoystick();
	//sliders mySliders = getSliders();
    //int8_t joyButton = buttonPressed(JOYSTICK);
    //int8_t leftButton = buttonPressed(LEFTBUTTON);
    //int8_t rightButton = buttonPressed(RIGHTBUTTON);

    CANmessage msg;
    msg.id = GAME_MESSAGE;		// Max 0x07ef
    msg.length = 3;
    msg.dataBytes[0] = game->flags.byte;
    msg.dataBytes[1] = myJoystick.xPos;
	msg.dataBytes[2] = game->lives;

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
