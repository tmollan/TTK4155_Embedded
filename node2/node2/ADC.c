
// Includes
#include "ADC.h"

void initADC(void) {
	makeInput(DDRF, PINF0);
	

	setBit(ADCSRA, ADATE);
	setBit(ADMUX, REFS0);

	
	// Left adjustment (for only reading 8-bit)
	setBit(ADMUX, ADLAR);
	
	// Enable ADC
	setBit(ADCSRA, ADEN);
	
	// Start A2D conversions
	setBit(ADCSRA, ADSC);
	
	
}


int8_t ball = 0;
int8_t ballDetected(void) {
	if (ADCH < 50 && ball == 0) {
		ball = 1;
		return 1;
	} else if(ADCH > 50 && ball == 1) {
		ball = 0;
	}
	return 0;
}