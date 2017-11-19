/*

EEPROM.h

*/

#ifndef EEPROM_H_
#define EEPROM_H_


// Functions
void writeEEPROM(uint16_t address, uint8_t data);   // Writes to EEPROM
uint8_t readEEPROM(uint16_t address);               // Reads from EEPROM


#endif /* EEPROM_H_ */
