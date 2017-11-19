
// Includes
#include "PWM.h"

// Initializes PWM output to servo
void initPWM(void) {
	// Initialize output pin
	makeOutput(DDR_PWM, PWMPIN);	// Servo pin

	// Set timer mode to fast PWM with ICR register defining TOP value
	clearBit(TCCR1A, WGM10);
	setBit(TCCR1A, WGM11);
	setBit(TCCR1B, WGM12);
	setBit(TCCR1B, WGM13);

	// Set compare output mode to non-inverting
	setBit(TCCR1A, COM1A1);
	clearBit(TCCR1A, COM1A0);

	// Set clock prescaling to clk/8 (8: prescaler value)
	clearBit(TCCR1B, CS10);
	setBit(TCCR1B, CS11);
	clearBit(TCCR1B, CS12);

	// Set TOP PWM counter value
	ICR1 = PWM_TOP;

	driveServo(0);		// Center servo
}

void driveServo(int8_t pos) {
	// Safety feature for servo motor
	if (pos < -100) {
		pos = -100;
	} else if (pos > 100) {
		pos = 100;
	}
	
	// Sets PWM timer compare value
	uint16_t midPos = PWM_TOP * 1.5/20;
	int16_t corr = PWM_TOP * 0.6/20 * pos/100;

	OCR1A = midPos + corr;
}
