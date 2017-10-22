/*

menu.h

*/

#ifndef MENU_H_
#define MENU_H_

// Includes
#include "setup.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <util/delay.h>
#include "OLED.h"
#include "joystick.h"

// Global variables
extern volatile int8_t menuMode, currentMenuIndex;
extern volatile enum application currentApp;

// Typedefs
typedef enum application {
    APP_NONE,       // No connected application
    APP_CALIBRATE,
    APP_DRAW,
    APP_FONTSIZE
} application;

typedef struct menupage {
    int8_t index;               // An index to identify index on prev menu
    int8_t length;				// Number of items in menu
    struct menuitem **items;	// Array of items
    struct menupage *prevPage;	// Previous menu
} menupage;

typedef struct menuitem {
    char *name;					// String to display in menu
    struct menupage *nextPage;	// The menu the item links to
    application app;            // Application to launch when entered
} menuitem;

// Functions
void initMenu(menupage **currentMenu);          // Initializes menu attributes
void loadMenu(int8_t menuIndex, menupage *currentMenu);     // Loads a given menu
void gotoNextMenu(int8_t index, menupage **currentMenu);    // Enter selected submenu
void gotoPrevMenu(menupage **currentMenu);      // Leave submenu and go back
void exitMenu(void);                            // Exit menu and clear page
void navigateMenu(menupage **currentMenu);      // Menu navigation
menupage * newMenu(menupage *prevPage);         // Create new menu page
menuitem * newItem(char *name, menupage *nextPage, application app);    // Create new menu item


#endif /* MENU_H_ */
