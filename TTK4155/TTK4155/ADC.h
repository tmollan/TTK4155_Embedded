/*

ADC.h

*/

#ifndef ADC_H_
#define ADC_H_

// Includes
#include "setup.h"			// Because delay needs F_OSC or F_CPU (?)
#include <util/delay.h>		// For _delay_ms, _delay_us, etc..
#include <stdint.h>			// For uint8_t, uint16_t, etc..


uint8_t readADC(uint8_t channel);	// Reads from ADC


#endif /* ADC_H_ */
