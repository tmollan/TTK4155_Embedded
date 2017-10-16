/*

SPI.h

*/


#ifndef SPI_H_
#define SPI_H_

#include "setup.h"		// For makeInput/-Output, setBit and loopUntilSet
#include <avr/io.h>
#include <stdint.h>		// For uint8_t
#include <util/delay.h>

void initSPI(void);
void writeSPI(uint8_t data);
uint8_t readSPI(void);


#endif /* SPI_H_ */
