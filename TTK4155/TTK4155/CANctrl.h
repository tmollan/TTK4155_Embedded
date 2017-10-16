/*

CANCTRL.h

*/


#ifndef CANCTRL_H_
#define CANCTRL_H_

// Includes
#include <avr/io.h>
#include "SPI.h"

void writeCAN(uint8_t address, uint8_t data);
uint8_t readCAN(uint8_t address);
uint8_t readStatusCAN(void);
void sendRequestCAN(uint8_t buffers);
void modifyBitCAN(uint8_t address, uint8_t mask, uint8_t data);
void resetCAN(void);


#endif /* CANCTRL_H_ */
