/*

EEPROM.h

*/

#ifndef EEPROM_H_
#define EEPROM_H_

// Includes
#include "setup.h"			// For setBit(), loopUntilClear(), etc..
#include <avr/io.h>			// For use of defined AVR registers and bits
#include <stdint.h>			// For uint8_t, uint16_t, etc..

// Functions
void writeEEPROM(uint16_t address, uint8_t data);   // Writes to EEPROM
uint8_t readEEPROM(uint16_t address);               // Reads from EEPROM


#endif /* EEPROM_H_ */
