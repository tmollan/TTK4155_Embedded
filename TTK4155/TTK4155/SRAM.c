
// Includes
#include "SRAM.h"


// Initializes SRAM
void initSRAM(void) {
	setBit(MCUCR, SRE);			// Enables the external memory interface
	setBit(SFIOR, XMM2);		// Masks out PC4-PC7 for the JTAG interface

	printf("SRAM external memory enabled\n");
}


// Writes data (0x00-0xFF) to an address (0x000-0x800) on the SRAM
void writeSRAM(uint16_t address, uint8_t data) {
	volatile char *ext_ram = (char*) 0x1800;		// Start address for the SRAM
	ext_ram[address] = data;						// Writes data to address
}


// Reads data (0x00-0xFF) from an address (0x000-0x800) on the SRAM
uint8_t readSRAM(uint16_t address) {
	volatile char *ext_ram = (char*) 0x1800;		// Start address for the SRAM
	return ext_ram[address];						// Reads data from address
}


// testSRAM() verifies that the SRAM IC is working properly
// by comparing sent data with recieved data
void testSRAM(void) {
	volatile char *ext_ram = (char*) 0x1800;		// Start address for the SRAM
	uint16_t ext_ram_size = 0x800;					// 0x800 addresses (* 8 bit = 2kB)
	uint16_t write_errors = 0;
	uint16_t retrieval_errors = 0;

	printf("Starting SRAM test...\n");

	// rand() stores some internal state, so calling this function in a loop will
	// yield different seeds each time (unless srand() is called before this function)
	uint16_t seed = rand();

	// Write phase: Immediately check that the correct value was stored
	srand(seed);

	for (uint16_t i = 0; i < ext_ram_size; i++) {
		uint8_t some_value = rand();
		ext_ram[i] = some_value;
		uint8_t retreived_value = ext_ram[i];
		if (retreived_value != some_value) {
			printf("Write phase error: ext_ram[%4d] = %02X (should be %02X)\n", i, retreived_value, some_value);
			write_errors++;
		}
	}

	// Retrieval phase: Check that no values were changed during or after the write phase
	srand(seed);

	// reset the PRNG to the state it had before the write phase
	for (uint16_t i = 0; i < ext_ram_size; i++) {
		uint8_t some_value = rand();
		uint8_t retreived_value = ext_ram[i];
		if (retreived_value != some_value) {
			printf("Retrieval phase error: ext_ram[%4d] = %02X (should be %02X)\n", i, retreived_value, some_value);
			retrieval_errors++;
		}
	}
	printf("SRAM test completed with\n%4d errors in write phase and\n%4d errors in retrieval phase\n\n", write_errors, retrieval_errors);
}
