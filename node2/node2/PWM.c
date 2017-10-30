
// Includes
#include "PWM.h"

//#define F_PWM F_CLK/N/(1+TOP)		// N: prescale factor (1, 8, 64, 256, 1024)
#define PWM_TOP_VALUE 0x9C3F

void initPWN(void) {
	// Initialize output pin
	makeOutput(DDR_PWM, PWMPIN);
	
	// Set up timer with suitable prescaler and CTC mode
	// Initialize counter
	// Initialize compare value
	// Enable compare interrupt
	// Enable global interrupts	
	
	// Set timer mode to fast PWM with ICR register defining TOP value
	clearBit(TCCR1A, WGM10);
	setBit(TCCR1A, WGM11);
	setBit(TCCR1B, WGM12);
	setBit(TCCR1B, WGM13);
	
	// Set compare output mode to non-inverting
	setBit(TCCR1A, COM1A1);
	clearBit(TCCR1A, COM1A0);
	
	// Set clock prescaling to clk/8
	clearBit(TCCR1B, CS10);
	setBit(TCCR1B, CS11);
	clearBit(TCCR1B, CS12);
	
	// Set TOP value
	ICR1H = PWM_TOP_VALUE >> 8;
	ICR1L = (uint8_t)PWM_TOP_VALUE;
}

void driveServo(int8_t pos) {
	if (pos < -100) {
		pos = -100;
	} else if (pos > 100) {
		pos = 100;
	}
	uint16_t midPos = PWM_TOP_VALUE * 0.075;
	int16_t corr = PWM_TOP_VALUE * 0.03 * pos/100;
	OCR1AH = (midPos + corr) >> 8;
	OCR1AL = (uint8_t)(midPos + corr);
}