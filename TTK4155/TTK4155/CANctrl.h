/*

CANCTRL.h

*/

#ifndef CANCTRL_H_
#define CANCTRL_H_


// CANctrl includes
#include "SPI.h"        // For select-/releaseSlave and defines


// CANctrl defines
// SPI instructions
#define WRITE_INST 0x02
#define READ_INST 0x03
#define READSTATUS_INST 0xA0
#define RTS_INST 0x80
#define BITMOD_INST 0x05
#define RESET_INST 0xC0


// CANctrl funstions
void writeCAN(uint8_t address, uint8_t data);
uint8_t readCAN(uint8_t address);
uint8_t readStatusCAN(void);
void sendRequestCAN(uint8_t buffers);
void modifyBitCAN(uint8_t address, uint8_t mask, uint8_t data);
void resetCAN(void);


#endif /* CANCTRL_H_ */
