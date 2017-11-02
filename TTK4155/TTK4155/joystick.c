
// Includes
#include "joystick.h"


// Channels for reading data from USB multifunction card
uint8_t yPosCh = 0x04;			// ch1: y-position of joystick
uint8_t xPosCh = 0x05;			// ch2: x-position of joystick
uint8_t lSliderCh = 0x06;		// ch3: position of left slider
uint8_t rSliderCh = 0x07;		// ch4: position of right slider

// For calibration of joystick
uint8_t xMidPos = 0x00, yMidPos = 0x00;
uint8_t xMax = 0x00, yMax = 0x00, xMin = 0xFF, yMin = 0xFF;


void initJoystick(void) {
	// Sets pins on MCU to inputs
	makeInput(DDRB, PINB0);		// Joystick button
	makeInput(DDRB, PINB1);		// Left touch button
	makeInput(DDRB, PINB2);		// Right touch button

	// Enables internal pull-ups
	setBit(PORTB, PINB0);		// Joystick button
	setBit(PORTB, PINB1);		// Left touch button
	setBit(PORTB, PINB2);		// Right touch button

	// Checks if joystick has been calibrated
	if (readEEPROM(EEAJOYINIT) == 0x01) {
		getParameters();		// Gets joystick parameters
	} else {
		calibrateJoystick();	// Calibrates joystick
	}
	printf("Joystick calibrated\n");
}


int getAngle(void) {
	// Reads joystick positions (0 to 255) and subtracts to get (~-128 to 127)
	int16_t xPos = readADC(XCH) - xMidPos;
	int16_t yPos = readADC(YCH) - yMidPos;

	// Deadzone: don't return angle if joystick is neutral
	if ((xPos > -10 && xPos < 10) && (yPos > -10 && yPos < 10)) {
		return -1;
	}

	int angle = 180 * atan2(yPos, xPos)/M_PI;	// Returns angle in degrees [-180, 180]
	if (angle < 0) {angle += 360;}				// Converts to [0, 360]

	return angle;		// Return angle in degrees [0, 360]
}


joystick getJoystick(void) {
	joystick js;

	// Assigns joystick positions (-100% to 100%)
	int16_t xPos = readADC(XCH) - xMidPos;
	int16_t yPos = readADC(YCH) - yMidPos;

	if (xPos > 0) {
		js.xPos = 100 * xPos / (xMax - xMidPos);
	} else {
		js.xPos = 100 * xPos / (xMidPos - xMin);
	}
	if (yPos > 0) {
		js.yPos = 100 * yPos / (yMax - yMidPos);
	} else {
		js.yPos = 100 * yPos / (yMidPos - yMin);
	}

	// Sets direction of joystick based on the angle
	js.angle = getAngle();
	if ((js.angle <= 45 || js.angle > 315) && js.angle != -1) {
		js.dir = RIGHT;
	} else if (js.angle > 45 && js.angle <= 135) {
		js.dir = UP;
	} else if (js.angle > 135 && js.angle <= 225) {
		js.dir = LEFT;
	} else if (js.angle > 225 && js.angle <= 315) {
		js.dir = DOWN;
	} else{
		js.dir = NEUTRAL;
	}

	return js;
}


sliders getSliders(void) {
	sliders s;

	// Assigns slider positions (-100% to 100%)
	s.leftPos = readADC(LSCH);		// Reads left slider (0-255)
	s.rightPos = readADC(RSCH);		// Reads right slider (0-255)

	s.leftPos = 100 * s.leftPos/255;	// Converts to percent
	s.rightPos = 100 * s.rightPos/255;	// Converts to percent

	return s;
}

// Returns 1 if the given button is pressed, 0 if not
int8_t buttonPressed(button b) {
	switch (b) {
		case JOYSTICK:
			return !testBit(PINB, PINB0);	// Joystick is normally closed
		case LEFTBUTTON:
			return testBit(PINB, PINB1);
		case RIGHTBUTTON:
			return testBit(PINB, PINB2);
		default:
			return 0;
	}
}


void calibrateJoystick(void) {
	printf("Calibrating joystick!\n");
	// Reads center position 100 times and finds average
	int xTemp = 0, yTemp = 0;
	for (int8_t i = 0; i < 100; i++) {
		xTemp += readADC(XCH);		// x-pos. of neutral joystick
		yTemp += readADC(YCH);		// y-pos. of neutral joystick
	}
	xMidPos = xTemp/100;
	yMidPos = yTemp/100;

	// Calibrates the max and min positions
	printf("Move joystick around, and press down when done.\n");

	int8_t tempFont = currentFont;
	setFont(FONT_NORMAL);
	char *string = "Calibrating joystick!\nMove joystick around, and press down when done.";
	uint16_t address = posAddressSRAM(1, 0);
	drawStringSRAM(string, address);
	refreshOLED();
	setFont(tempFont);

	while(!buttonPressed(JOYSTICK)) {
		uint8_t xPos = readADC(XCH);
		uint8_t yPos = readADC(YCH);

		if (xPos > xMax) xMax = xPos;
		if (xPos < xMin) xMin = xPos;
		if (yPos > yMax) yMax = yPos;
		if (yPos < yMin) yMin = yPos;
	}

	// Writes obtained parameters to EEPROM
	writeEEPROM(EEAXMAX, xMax);		// Maximum value of x
	writeEEPROM(EEAXMIN, xMin);		// Minimum value of x
	writeEEPROM(EEAXMID, xMidPos);	// Center value of x
	writeEEPROM(EEAYMAX, yMax);		// Maximum value of y
	writeEEPROM(EEAYMIN, yMin);		// Minimum value of y
	writeEEPROM(EEAYMID, yMidPos);	// Center value of y
	writeEEPROM(EEAJOYINIT, 0x01);	// Indicate that joystick has been calibrated
	printf("Parameters saved to EEPROM\n");
}

// Gets parameters from EEPROM when joystick already has been calibrated
void getParameters(void) {
	xMax = readEEPROM(EEAXMAX);		// Maximum value of x
	xMin = readEEPROM(EEAXMIN);		// Minimum value of x
	xMidPos = readEEPROM(EEAXMID);	// Center value of x
	yMax = readEEPROM(EEAYMAX);		// Maximum value of y
	yMin = readEEPROM(EEAYMIN);		// Minimum value of y
	yMidPos = readEEPROM(EEAYMID);	// Center value of y
}
