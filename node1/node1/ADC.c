
// Includes
#include "setup.h"			// Because delay needs F_OSC or F_CPU (?)
#include <util/delay.h>		// For _delay_ms, _delay_us, etc..
#include <stdint.h>			// For uint8_t, uint16_t, etc..


// Reads data (0x00-0xFF) from address (0x000) on the ADC
// Which channel we'd like to read must be written to the same address first
uint8_t readADC(uint8_t channel) {
	volatile char *adc = (char*) 0x1400;	// Start address for the ADC
	adc[0x000] = channel;					// Tells ADC to read from 'channel'
	_delay_us(50);							// 50 us delay while ADC reads
	return adc[0x000];						// Returns data from 'channel'
}
