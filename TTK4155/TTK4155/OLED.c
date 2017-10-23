
// Includes
#include <stdint.h>
#include <stdio.h>			// For printf()
#include <stdlib.h>
#include <math.h>
#include <util/delay.h>
#include <string.h>
#include "OLED.h"
#include "fonts.h"
#include "animations.h"
#include "joystick.h"
#include "SRAM.h"
#include "menu.h"


// Global variables
volatile positionOLED cursor;
volatile int8_t currentFont;
volatile int8_t fontWidth;


// Recommended Software Initialization from product specification data sheet
void initOLED(void) {
	writeCommandOLED(0xAE);	// Display OFF (sleep mode)

	writeCommandOLED(0xA1);	// Segment re-map (column address 127 is mapped to SEG0)

	writeCommandOLED(0xDA);	// Set COM pins
	writeCommandOLED(0x12);	// Alternative????????????

	writeCommandOLED(0xA8);	// Set multiplex ratio
	writeCommandOLED(0x3F);	// N+1 MUX (63 -> 64MUX)

	// Set COM output scan direction
	writeCommandOLED(0xC8);	// Remapped mode: Scan from COM[N-1] to COM[0]

	writeCommandOLED(0xD5);	// Set display clock
	writeCommandOLED(0x80);	// Divide ratio/oscillator frequency ????

	writeCommandOLED(0x81);	// Set contrast control
	writeCommandOLED(0x50); // 0-255 (contrast increases with value)

	writeCommandOLED(0xD9);	// Set pre-charge period
	writeCommandOLED(0x21); // ???

	writeCommandOLED(0x20);	// Set memory addressing mode
	writeCommandOLED(0x02);	// Page addressing mode

	// For horizontal or vertical addressing mode only?
	writeCommandOLED(0x21);	// Set column address
	writeCommandOLED(0x00);	// Column start address: 0-127
	writeCommandOLED(0x7F); // Column end address: 0-127

	// For horizontal or vertical addressing mode only?
	writeCommandOLED(0x22);	// Set page address
	writeCommandOLED(0x00);	// Page start address: 0-7
	writeCommandOLED(0x07);	// Page end address: 0-7

	writeCommandOLED(0xDB);	// Set V_COMH deselect level
	writeCommandOLED(0x30); // ~083 x V_CC

	writeCommandOLED(0xAD);	// External (0x00) or internal (0x10) I_REF selection
	writeCommandOLED(0x00); // External

	writeCommandOLED(0xA4);	// Output follows RAM content

	writeCommandOLED(0xA6);	// Set normal/inverse display (0xA6/0xA7)
	writeCommandOLED(0xAF);	// Display ON (normal mode)

	setFont(FONT_NORMAL);
	clearDisplayOLED();
	clearDisplaySRAM();
	resetCursor();

	printf("OLED display ON\n");
}

// Write command to change settings on OLED
void writeCommandOLED(uint8_t command) {
	volatile char *oled_cmd = (char*) 0x1000;
	oled_cmd[0x000] = command;
}

// Write data to display on OLED
void writeDataOLED(uint8_t data) {
	volatile char *oled_data = (char*) 0x1200;
	oled_data[0x000] = data;
}

// Move cursor on OLED to a given page (y-axis: 0-top, 7-bottom)
void gotoPageOLED(uint8_t page) {
		writeCommandOLED(0xB0 + page);
}

// Move cursor on OLED to a given column (x-axis: 0-127)
void gotoColumnOLED(uint8_t column) {
	writeCommandOLED(0x00 + column%16);
	writeCommandOLED(0x10 + column/16);
}

// Move cursor to top left (page 0, column 0)
void gotoHomeOLED(void) {
	gotoPageOLED(0);
	gotoColumnOLED(0);
}

// Move cursor to a given position
void gotoPosOLED(uint8_t page, uint8_t column) {
	gotoPageOLED(page);
	gotoColumnOLED(column);
}

// Clears a page on OLED (by writing zeros)
void clearPageOLED(uint8_t page) {
	gotoPageOLED(page);
	gotoColumnOLED(0);
	for (uint8_t col = 0; col < 128; col++) {
		writeDataOLED(0x00);
	}
}

// Clears the display by iterating all pages
void clearDisplayOLED(void) {
	gotoHomeOLED();
	for (uint8_t page = 0; page < 8; page++) {
		clearPageOLED(page);
	}
	gotoHomeOLED();
}

// Draws a character on the OLED
void drawCharOLED(char ch) {
	ch -= 32;			// Gets the font value of the character
	for (int8_t col = 0; col < fontWidth; col++) {
		uint8_t data = pgm_read_byte(&font8[(int8_t)ch][col]);
		writeDataOLED(data);
	}
}

// Draws a string on the OLED by iterating each character in the string
void drawStringOLED(char *string) {
	int i = 0;
	// Draws each character in the string
	while (string[i] != '\0') {
		drawCharOLED(string[i]);
		i++;
	}
}


// ----------------------------------------------
// SRAM - SRAM - SRAM - SRAM - SRAM - SRAM - SRAM
// The following functions writes to SRAM before
// displaying SRAM data on OLED
// ----------------------------------------------

// Returns the SRAM address of a given OLED page and column
uint16_t posAddressSRAM(uint8_t page, uint8_t column) {
	return page*128 + column;
}

// Refresh OLED with data on SRAM
void refreshOLED(void) {
	for (uint8_t page = 0; page < 8; page++) {
		gotoPageOLED(page);
		gotoColumnOLED(0);
		for (uint8_t col = 0; col < 128; col++) {
			uint16_t address = page*128 + col;
			uint8_t data = readSRAM(address);
			writeDataOLED(data);
		}
	}
}

// Clears the addresses for a given page on SRAM
void clearPageSRAM(uint8_t page) {
	uint16_t startAddress = page*128;
	for (uint8_t adr = 0; adr < 128; adr++) {
		writeSRAM(startAddress + adr, 0x00);
	}
}

// Clears all OLED addresses on SRAM
void clearDisplaySRAM(void) {
	for (uint8_t page = 0; page < 8; page++) {
		clearPageSRAM(page);
	}
}

// Draws a character to a given SRAM address
void drawCharSRAM(char ch, uint16_t address) {
	ch -= 32;		// Gets the font value of the character
	for (int8_t col = 0; col < fontWidth; col++) {
		uint8_t data = 0x00;
		switch (currentFont) {
			case FONT_BIG:
				data = pgm_read_byte(&font8[(int8_t)ch][col]);
				break;
			case FONT_NORMAL:
				data = pgm_read_byte(&font5[(int8_t)ch][col]);
				break;
			case FONT_SMALL:
				data = pgm_read_byte(&font4[(int8_t)ch][col]);
				break;
		}
		writeSRAM(address + col, data);
	}
}

// Draws a string by iterating over the characters of the string
void drawStringSRAM(char *string, uint16_t address) {
	int i = 0;
	int j = 0;
	// Draws each character in a the string
	while (string[i] != '\0') {
		// Go to next page for each new line
		if (string[i] == '\n') {
			address += (128 - j*fontWidth) - fontWidth;
			i++;
			j = 0;
			continue;
		}
		// Start on top of screen if address exceeds address space
		if (address > 1023) {
			address = 0x00;
		}
		drawCharSRAM(string[i], address + i*fontWidth);
		i++;
		j++;
	}
}

// Draws a character in inverted color to a given SRAM address
void drawInvCharSRAM(char ch, uint16_t address) {
	ch -= 32;			// Gets the font value of the character
	for (int8_t col = 0; col < fontWidth; col++) {
		uint8_t data = 0x00;
		switch (currentFont) {
			case FONT_BIG:
				data = ~pgm_read_byte(&font8[(int8_t)ch][col]);		// ~: inverts bits
				break;
			case FONT_NORMAL:
				data = ~pgm_read_byte(&font5[(int8_t)ch][col]);
				break;
			case FONT_SMALL:
				data = ~pgm_read_byte(&font4[(int8_t)ch][col]);
				break;
		}
		writeSRAM(address + col, data);
	}
}

// Draws a string in inverted color by iterating over the characters of the string
void drawInvStringSRAM(char *string, uint16_t address) {
	int i = 0;
	int j = 0;
	// Draws each character in a the string
	while (string[i] != '\0') {
		// Go to next page for each new line
		if (string[i] == '\n') {
			address += (128 - j*fontWidth) - fontWidth;
			i++;
			j = 0;
			continue;
		}
		// Start on top of screen if address exceeds address space
		if (address > 1023) {
			address = 0x00;
		}
		drawInvCharSRAM(string[i], address + i*fontWidth);
		i++;
		j++;
	}
}

// Draws a pixel at given coordinates, leave the rest of the bits in the byte
void drawPixelSRAM(uint8_t x, uint8_t y) {
	if (x < 128 && y < 64) {
		y = 63 - y;
		uint16_t address = y/8*128 + x;		// page = y/8
		uint8_t data = readSRAM(address);
		data |= (1 << y%8);
		writeSRAM(address, data);
	}
}

// Draws a line from (x0, y0) to (x1, y1)
void drawLineSRAM(int8_t x0, int8_t y0, int8_t x1, int8_t y1) {
	int8_t distance = 0;
	if (abs(x1 - x0) >= abs(y1 - y0)) {
		distance = abs(x1 - x0);
	} else {
		distance = abs(y1 - y0);
	}

	float ax = (float)(x1 - x0)/distance;
	float ay = (float)(y1 - y0)/distance;

	float x = (float)x0;
	float y = (float)y0;

	for (uint8_t i = 0; i < distance; i++) {
		drawPixelSRAM((uint8_t)x, (uint8_t)y);
		x += ax;
		y += ay;
	}
}

// Draws a circle with origin (x0, y0) and radius r
void drawCircleSRAM(int8_t x0, int8_t y0, int8_t r) {
	float x, y;
	float theta = 0.0;
	for (int t = 0; t < 2*M_PI*r; t++) {
		theta = (float)t/r;
		x = r*cos(theta) + x0;
		y = r*sin(theta) + y0;
		drawPixelSRAM((uint8_t)x, (uint8_t)y);
	}
}

// Drawing with the joystick
void drawJoystickSRAM(void) {
	joystick js;
	js = getJoystick();

	// Prints cursor coordinates in top right corner
	char string[10];
	sprintf(string, " (%d,%d)", cursor.xPos, cursor.yPos);
	drawStringSRAM(string, 127-strlen(string)*fontWidth);

	if (js.dir != NEUTRAL) {

		// 8-directional movements
		if (js.angle < 23 || js.angle >= 338) {				// East
			incCursorX();
		} else if (js.angle >= 23 && js.angle < 68) {		// North-east
			incCursorX();
			incCursorY();
		} else if (js.angle >= 68 && js.angle < 113) {		// North
			incCursorY();
		} else if (js.angle >= 113 && js.angle < 158) {		// North-west
			decCursorX();
			incCursorY();
		} else if (js.angle >= 158 && js.angle < 203) {		// West
			decCursorX();
		} else if (js.angle >= 203 && js.angle < 248) {		// South-west
			decCursorX();
			decCursorY();
		} else if (js.angle >= 248 && js.angle < 293) {		// South
			decCursorY();
		} else if (js.angle >= 293 && js.angle < 338) {		// South-east
			incCursorX();
			decCursorY();
		}

		/*// Square movements
		switch (js.dir) {
			case RIGHT:
				incCursorX();
				break;
			case UP:
				incCursorY();
				break;
			case LEFT:
				decCursorX();
				break;
			case DOWN:
				decCursorY();
				break;
		}*/

		drawPixelSRAM(cursor.xPos, cursor.yPos);
	}
}

// Resets cursor position to top left corner
void resetCursor(void) {
	cursor.xPos = 0;
	cursor.yPos = 63;
	cursor.page = 0;
	cursor.column = 0;
}

// Increases x-position of cursor by one
void incCursorX(void) {
	if (cursor.xPos >= 0 && cursor.xPos < 127) {
		cursor.xPos += 1;
		cursor.column = cursor.xPos;
	}
}

// Decreases x-position of cursor by one
void decCursorX(void) {
	if (cursor.xPos > 0 && cursor.xPos <= 127) {
		cursor.xPos -= 1;
		cursor.column = cursor.xPos;
	}
}

// Increases y-position of cursor by one
void incCursorY(void) {
	if (cursor.yPos >= 0 && cursor.yPos < 63) {
		cursor.yPos += 1;
		cursor.page = (63 - cursor.yPos)/8;
	}
}

// Decreases y-position of cursor by one
void decCursorY(void) {
	if (cursor.yPos > 0 && cursor.yPos <= 63) {
		cursor.yPos -= 1;
		cursor.page = (63 - cursor.yPos)/8;
	}
}

// Sets cursor to a given page and column
void setCursor(uint8_t page, uint8_t column) {
	if (page >= 0 && page < 8) {
		cursor.page = page;
		cursor.yPos = 63 - page*8;
	}
	if (column >= 0 && column < 128) {
		cursor.column = column;
		cursor.xPos = column;
	}
}

// Draws an animated check mark
void drawCheckMark(void) {
	for (int16_t i = 0; i < 1024; i++) {
		uint8_t data = pgm_read_byte(&checkmark[i]);
		writeSRAM(i, pgm_read_byte(&BitReversalTable256[data]));
	}
	refreshOLED();
	_delay_ms(5000);
}

/*
void drawStartScreen(void) {
	for (int16_t i = 0; i < 1024; i++) {
		uint8_t data = pgm_read_byte(&startscreen[i]);
		writeSRAM(i, pgm_read_byte(&BitReversalTable256[data]));
	}
	refreshOLED();
}
*/

// Changes the global font and fontWidth
void setFont(font size) {
	switch (size) {
		case FONT_BIG:
			currentFont = FONT_BIG;
			fontWidth = 8;
			break;
		case FONT_NORMAL:
			currentFont = FONT_NORMAL;
			fontWidth = 5;
			break;
		case FONT_SMALL:
			currentFont = FONT_SMALL;
			fontWidth = 4;
			break;
	}
}
