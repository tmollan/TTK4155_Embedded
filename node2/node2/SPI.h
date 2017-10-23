/*

SPI.h

*/

#ifndef SPI_H_
#define SPI_H_

// SPI includes
#include "setup.h"		// For makeInput/-Output, setBit and loopUntilSet
#include <avr/io.h>
#include <stdint.h>		// For uint8_t
#include <stdio.h>


// SPI defines
#define DUMMY 0x00
// Pins and ports
#define DDR_SPI DDRB    // Data Direction Register for SPI
#define PORT_SPI PORTB  // Data Register for SPI
#define SSPIN PINB7     // Slave Select pin
#define MOSIPIN PINB2   // Master Output/Slave Input pin
#define MISOPIN PINB3   // Master Input/Slave Output pin
#define SCKPIN PINB1    // SPI Bus Serial Clock pin


// SPI functions
void initSPI(void);
uint8_t transmitSPI(uint8_t data);
void selectSlave(void);
void releaseSlave(void);


#endif /* SPI_H_ */
