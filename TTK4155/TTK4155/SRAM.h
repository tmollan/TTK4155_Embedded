/*

SRAM.h

*/

#ifndef SRAM_H_
#define SRAM_H_

// Includes
#include "setup.h"			// For setBit(), loopUntilSet(), etc..
#include <avr/io.h>			// For use of defined AVR registers and bits
#include <stdlib.h>			// For rand()
#include <stdint.h>			// For uint8_t, uint16_t, etc..
#include <stdio.h>			// For printf()

// Functions
void initSRAM(void);                            // Initializes SRAM
void writeSRAM(uint16_t address, uint8_t data); // Writes to SRAM
uint8_t readSRAM(uint16_t address);             // Reads from SRAM

void testSRAM(void);        // Verifies that SRAM IC is working


#endif /* SRAM_H_ */
