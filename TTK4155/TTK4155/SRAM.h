/*

SRAM.h

*/

#ifndef SRAM_H_
#define SRAM_H_

// Includes


// Functions
void initSRAM(void);                            // Initializes SRAM
void writeSRAM(uint16_t address, uint8_t data); // Writes to SRAM
uint8_t readSRAM(uint16_t address);             // Reads from SRAM

//void testSRAM(void);        // Verifies that SRAM IC is working


#endif /* SRAM_H_ */
