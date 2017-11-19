
// Includes
#include "ADC.h"

// Initializes AD Converter
void initADC(void) {
	makeInput(DDRF, PINF0);		// Pin for reading IR value

	setBit(ADCSRA, ADATE);		// Enable auto conversion triggering
	setBit(ADMUX, REFS0);		// Voltage reference for ADC
	setBit(ADMUX, ADLAR);		// Left adjustment (for only reading 8-bit)
	setBit(ADCSRA, ADEN);		// Enable ADC
	setBit(ADCSRA, ADSC);		// Start A2D conversions
}

uint8_t readADC(void) {
	return ADCH;	// Sufficient to read only high byte when left adjusted
}
