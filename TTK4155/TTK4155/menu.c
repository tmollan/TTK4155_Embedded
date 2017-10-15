
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
  menuitem **homeItems = malloc(2 * sizeof(menuitem*));
  homeItems[0] = newItem("Play game", playPage, NONE);
  homeItems[1] = newItem("Options", optionsPage, NONE);
  homePage->items = malloc(sizeof(homeItems));
  homePage->items = homeItems;
  homePage->length = 2;

  // List of items on play page
  menuitem **playItems = malloc(1 * sizeof(menuitem*));
  playItems[0] = newItem("Draw", NULL, DRAW);
  playPage->items = malloc(sizeof(playItems));
  playPage->items = playItems;
  playPage->length = 1;

  // List of items on option page
  menuitem **optionsItems = malloc(2 * sizeof(menuitem*));
  optionsItems[0] = newItem("Calibrate", NULL, CALIBRATE);
  optionsItems[1] = newItem("Change font", fontPage, NONE);
  optionsPage->items = malloc(sizeof(optionsItems));
  optionsPage->items = optionsItems;
  optionsPage->length = 2;

  // List of items on options->font page
  menuitem **fontItems = malloc(3 * sizeof(menuitem*));
  fontItems[0] = newItem("Big", NULL, FONTSIZE);
  fontItems[1] = newItem("Normal", NULL, FONTSIZE);
  fontItems[2] = newItem("Small", NULL, FONTSIZE);
  fontPage->items = malloc(sizeof(fontItems));
  fontPage->items = fontItems;
  fontPage->length = 3;

  // Menu mode at startup
  *menu = homePage;
  menuMode = 1;
  currentMenuIndex = 0;
  currentApp = NONE;
  loadMenu(currentMenuIndex, (*menu));
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
    (*menu)->index = menuIndex;   // To stay on selected index when coming back
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

// Navigates menu based on joystick position
void navigateMenu(menupage **menu) {
	joystick js = getJoystick();
	switch (js.dir) {

    // Move up on menu
		case UP:
			if (currentMenuIndex > 0) {
			    currentMenuIndex--;
  				loadMenu(currentMenuIndex, (*menu));
  				_delay_ms(250);
			}
			break;

    // Move down on menu
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
				if ((*menu)->items[currentMenuIndex]->app != NONE) {
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
  myPage->prevPage = prevPage;
  return myPage;
}

// Function to create a new list item on a menu page
menuitem * newItem(char *name, menupage *nextPage, application app) {
  menuitem *myItem = malloc(sizeof(menuitem));
  myItem->name = name;
  myItem->nextPage = nextPage;
  myItem->app = app;
  return myItem;
}