
// Includes
#include "setup.h"			// For setBit(), loopUntilClear(), etc..
#include <avr/io.h>			// For use of defined AVR registers and bits
#include <stdint.h>			// For uint8_t, uint16_t, etc..


// Writes data (0x00-0xFF) to an address (0x000-0x1FF) in EEPROM
void writeEEPROM(uint16_t address, uint8_t data) {
    loopUntilClear(EECR, EEWE);   // Wait for completion of previous write

    EEAR = address;       // Tells the address register which address to write to
    EEDR = data;          // Puts data to be written into the data register

    setBit(EECR, EEMWE);  // Sets EEMWE bit to enable EEPROM write
    setBit(EECR, EEWE);   // Sets EEWE bit to trigger EEPROM write
}


// Reads data (0x00-0xFF) from an address (0x000-0x1FF) in EEPROM
uint8_t readEEPROM(uint16_t address) {
    loopUntilClear(EECR, EEWE);    // Wait for completion of previous write

    EEAR = address;       // Tells the address register which address to read from

    setBit(EECR, EERE);   // Sets EERE bit to trigger EEPROM read
    return EEDR;          // Return data from data register EEDR
}
