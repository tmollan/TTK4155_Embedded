/*

ADC.h

*/

#ifndef ADC_H_
#define ADC_H_

// Includes
#include "setup.h"
#include <avr/io.h>
#include <stdint.h>


// Functions
void initADC(void);
uint8_t readADC(void);


#endif /* ADC_H_ */
