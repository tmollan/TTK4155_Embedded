
// Includes
#include "ADC.h"

void initADC(void) {
	// Pin for reading IR value
	makeInput(DDRF, PINF0);

	setBit(ADCSRA, ADATE);
	setBit(ADMUX, REFS0);

	// Left adjustment (for only reading 8-bit)
	setBit(ADMUX, ADLAR);

	// Enable ADC
	setBit(ADCSRA, ADEN);

	// Start A2D conversions
	setBit(ADCSRA, ADSC);	// Can be removed because of ADATE?
}

uint8_t readADC(void) {
	return ADCH;
}
