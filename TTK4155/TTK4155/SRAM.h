/*

SRAM.h

*/

#ifndef SRAM_H_
#define SRAM_H_


void initSRAM(void);    // Initializes SRAM
void testSRAM(void);    // Verifies that SRAM IC is working
void writeSRAM(uint16_t address, uint8_t data);    // Writes to SRAM
uint8_t readSRAM(uint16_t address);                // Reads from SRAM


#endif /* SRAM_H_ */
