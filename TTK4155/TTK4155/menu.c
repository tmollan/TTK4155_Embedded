
// Includes
#include "setup.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <util/delay.h>
#include "OLED.h"
#include "menu.h"
#include "joystick.h"


void initMenu(menupage **menu) {
    // Creates menu pages (with links to previous pages)
    menupage *homePage = newMenu(NULL);
    menupage *playPage = newMenu(homePage);
    menupage *optionsPage = newMenu(homePage);
    menupage *fontPage = newMenu(optionsPage);

    // List of items on home page
    //menuitem **homeItems = malloc(2 * sizeof(menuitem*));
    menuitem *homeItems[2];
    homeItems[0] = newItem("Play game", playPage, APP_NONE);
    homeItems[1] = newItem("Options", optionsPage, APP_NONE);
    //homePage->items = malloc(sizeof(homeItems));
    homePage->items = homeItems;
    homePage->length = 2;

    // List of items on play page
    //menuitem **playItems = malloc(2 * sizeof(menuitem*));
    menuitem *playItems[2];
    playItems[0] = newItem("Ping-pong", NULL, APP_GAME);
    playItems[1] = newItem("Draw", NULL, APP_DRAW);
    //playPage->items = malloc(sizeof(playItems));
    playPage->items = playItems;
    playPage->length = 2;

    // List of items on option page
    //menuitem **optionsItems = malloc(2 * sizeof(menuitem*));
    menuitem *optionsItems[2];
    optionsItems[0] = newItem("Calibrate", NULL, APP_CALIBRATE);
    optionsItems[1] = newItem("Change font", fontPage, APP_NONE);
    //optionsPage->items = malloc(sizeof(optionsItems));
    optionsPage->items = optionsItems;
    optionsPage->length = 2;

    // List of items on options->font page
    //menuitem **fontItems = malloc(3 * sizeof(menuitem*));
    menuitem *fontItems[3];
    fontItems[0] = newItem("Big", NULL, APP_FONTSIZE);
    fontItems[1] = newItem("Normal", NULL, APP_FONTSIZE);
    fontItems[2] = newItem("Small", NULL, APP_FONTSIZE);
    //fontPage->items = malloc(sizeof(fontItems));
    fontPage->items = fontItems;
    fontPage->length = 3;

    // Menu mode at startup
    *menu = homePage;
    menuMode = 1;
    currentMenuIndex = 0;
    currentApp = APP_NONE;
    loadMenu(currentMenuIndex, (*menu));

    printf("Menu loaded\n");
}

// Loads given menu with menuIndex selected
void loadMenu(int8_t menuIndex, menupage *menu) {
	clearDisplaySRAM();

    // Don't print more items than menu has
	int8_t maxIndex = 7;
	if (menu->length - 1 < maxIndex) {
		maxIndex = menu->length - 1;
	}
    // Adjust menu if menuIndex is larger than the amount of OLED pages
    int8_t startIndex = 0;
	if (menuIndex > 7) {
		startIndex = menuIndex - 7;
	}
    // Prints the menu items with the selected item inverted
	for (int8_t i = startIndex, page = 0; i <= startIndex + maxIndex; i++, page++) {
		if (i == menuIndex) {
			drawInvStringSRAM(menu->items[i]->name, page*128);
		} else {
			drawStringSRAM(menu->items[i]->name, page*128);
		}
	}
	refreshOLED();
}

// If there is a next page, go to it
void gotoNextMenu(int8_t menuIndex, menupage **menu) {
    if ((*menu)->items[menuIndex]->nextPage != NULL) {
        *menu = (*menu)->items[menuIndex]->nextPage;
        (*menu)->index = menuIndex;     // To stay on selected index when coming back
        currentMenuIndex = 0;
        loadMenu(currentMenuIndex, (*menu));
    }
}

// If there is a previous page, go back
void gotoPrevMenu(menupage **menu) {
    if ((*menu)->prevPage != NULL) {
        currentMenuIndex = (*menu)->index;    // Staying on same index as last time
        *menu = (*menu)->prevPage;
        loadMenu(currentMenuIndex, (*menu));
    }
}

// Exits menu and clears display
void exitMenu(void) {
    menuMode = 0;
    clearDisplaySRAM();
    refreshOLED();
}

// Exits app and enters menu
void exitApp(menupage *menu) {
    currentApp = APP_NONE;
    menuMode = 1;
    loadMenu(currentMenuIndex, menu);
}

// Change app
void gotoGameApp() {
	currentApp = APP_GAME;
}

// Navigates menu based on joystick position
void navigateMenu(menupage **menu) {
	joystick js = getJoystick();
	switch (js.dir) {

		case UP:
			if (currentMenuIndex > 0) {
			    currentMenuIndex--;
  				loadMenu(currentMenuIndex, (*menu));
  				_delay_ms(250);
			}
			break;

		case DOWN:
			if (currentMenuIndex < (*menu)->length-1) {
				currentMenuIndex++;
				loadMenu(currentMenuIndex, (*menu));
				_delay_ms(250);
			}
			break;

        // Leave submenu and go back to previous page
		case LEFT:
            gotoPrevMenu(menu);
            _delay_ms(250);
            break;

        // Enter submenu of selected index
		case RIGHT:
            gotoNextMenu(currentMenuIndex, menu);
            _delay_ms(250);
            break;

        // If selected index links to an application, exit menu and launch app.
		case NEUTRAL:
			if (buttonPressed(JOYSTICK)) {
				if ((*menu)->items[currentMenuIndex]->app != APP_NONE) {
					currentApp = (*menu)->items[currentMenuIndex]->app;
					exitMenu();
					_delay_ms(250);
				}
			}
	}
}

// Function to create a new menu page
menupage * newMenu(menupage *prevPage) {
    menupage *myPage = malloc(sizeof(menupage));
    if (myPage == NULL) return NULL;
    myPage->prevPage = prevPage;
    return myPage;
}

// Function to create a new list item on a menu page
menuitem * newItem(const char *name, menupage *nextPage, application app) {
    menuitem *myItem = malloc(sizeof(menuitem));
    if (myItem == NULL) {
        printf("malloc error\n");
        return NULL;
    }
    myItem->name = strdup(name);
    if (myItem->name == NULL) {
        printf("strdup error\n");
        free(myItem);
        return NULL;
    }
    myItem->nextPage = nextPage;
    myItem->app = app;
    return myItem;
}
