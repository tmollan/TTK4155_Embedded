/*

joystick.h

*/

#ifndef JOYSTICK_H_
#define JOYSTICK_H_

// Includes
#include "setup.h"			// For setBit(), makeInput(), etc..
#include <avr/io.h>			// For use of defined AVR registers and bits
#include <stdint.h>			// For uint8_t, uint16_t, etc..
#include <stdio.h>			// For printf()
#include <math.h>			// For atan2() (?) and M_PI (?)
#include <util/delay.h>
#include "ADC.h"			// For readADC()
#include "EEPROM.h"			// For read- and writeEEPROM()
#include "OLED.h"

// Defines
// Channels for reading data from USB multifunction card
#define YCH 0x04		// ch1: y-position of joystick
#define XCH 0x05		// ch2: x-position of joystick
#define LSCH 0x06		// ch3: position of left slider
#define RSCH 0x07		// ch4: position of right slider

// EEPROM addresses for joystick parameters
#define EEAXMAX 0x000		// Maximum value of x
#define EEAXMIN 0x001		// Minimum value of x
#define EEAXMID 0x002		// Center value of x
#define EEAYMAX 0x003		// Maximum value of y
#define EEAYMIN 0x004		// Minimum value of y
#define EEAYMID 0x005		// Center value of y
#define EEAJOYINIT 0x006 	// Indication of calibration

// Typedefs
// For assigning joystick directions with ease
typedef enum {
	RIGHT,		// 0
	UP,			// 1
	LEFT,		// 2
	DOWN,		// 3
	NEUTRAL		// 4
} direction;

// For returning button states
typedef enum {
	JOYSTICK,		// 0
	LEFTBUTTON,		// 1
	RIGHTBUTTON		// 2
} button;

// For declaring joystick variables
typedef struct {
	int xPos;		// x-position of joystick
	int yPos;		// y-position of joystick
	int angle;		// Angle of joystick
	direction dir;	// Direction enum: LEFT, RIGHT, UP, DOWN, NEUTRAL
} joystick;

// For declaring sliders variables
typedef struct {
	int leftPos;	// Position of left slider
	int rightPos;	// Position of right slider
} sliders;

// Functions
void initJoystick(void);		// Calibrates and initializes joystick
int getAngle(void);				// Returns angle of joystick
joystick getJoystick(void);		// Returns joystick struct (see above)
sliders getSliders(void);		// Returns sliders struct (see above)
int8_t buttonPressed(button b);	// Returns button states
void calibrateJoystick(void);	// Calibrates joystick
void getParameters(void);		// Gets parameters if joystick is calibrated


#endif /* JOYSTICK_H_ */
