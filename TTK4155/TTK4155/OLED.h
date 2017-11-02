/*

OLED.h

*/


#ifndef OLED_H_
#define OLED_H_

// Includes
#include <stdint.h>

// Global variables
extern volatile struct positionOLED cursor;
extern volatile int8_t currentFont;
extern volatile int8_t fontWidth;

// Typedefs
// For remembering positions
typedef struct positionOLED {
    uint8_t xPos;   // 0-127
    uint8_t yPos;   // 0-63
    uint8_t page;   // 0-7
    uint8_t column; // 0-127
} positionOLED;

typedef enum {
	FONT_BIG,
	FONT_NORMAL,
	FONT_SMALL
} font;

// Functions
// Initializes OLED
void initOLED(void);

// Direct OLED communication
void writeCommandOLED(uint8_t command);
void writeDataOLED(uint8_t data);

// Moves cursor to positions on OLED
void gotoPageOLED(uint8_t line);
void gotoColumnOLED(uint8_t column);
void gotoHomeOLED(void);
void gotoPosOLED(uint8_t page, uint8_t column);

void clearPageOLED(uint8_t page);   // Clears a page on OLED
void clearDisplayOLED(void);        // Clears display on OLED

// Draws to OLED
void drawCharOLED(char ch);
void drawStringOLED(char *string);

// SRAM OLED communication
uint16_t posAddressSRAM(uint8_t page, uint8_t column);  // Returns SRAM address

void refreshOLED(void);             // Refreshes OLED display with SRAM data
void clearPageSRAM(uint8_t page);   // Clears a page on SRAM
void clearDisplaySRAM(void);        // Clears display on SRAM

// Draws to SRAM at given SRAM address
void drawCharSRAM(char ch, uint16_t address);
void drawStringSRAM(char *string, uint16_t address);
void drawInvCharSRAM(char ch, uint16_t address);         // Inverted text
void drawInvStringSRAM(char *string, uint16_t address);  // Inverted text
void drawPixelSRAM(uint8_t x, uint8_t y);

// Geometric drawing functions
void drawLineSRAM(int8_t x0, int8_t y0, int8_t x1, int8_t y1);
void drawCircleSRAM(int8_t x0, int8_t y0, int8_t r);

// Use joystick to draw pixels
void drawJoystickSRAM(void);

// Cursor positioning
void resetCursor(void);  // Resets cursor position
void incCursorX(void);   // Increment x
void decCursorX(void);   // Decrement x
void incCursorY(void);   // Increment y
void decCursorY(void);   // Decrement y
void setCursor(uint8_t page, uint8_t column);

// Animates a check mark
void drawCheckMark(void);
void drawStartScreen(void);
void invertDisplaySRAM(void);

// Changes the global font
void setFont(font size);

#endif /* OLED_H_ */
