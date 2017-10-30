/*

CANCTRL.h

*/

#ifndef CANCTRL_H_
#define CANCTRL_H_

// Includes
#include "SPI.h"        // For select-/releaseSlave and defines
#include <util/delay.h>
#include <stdio.h>

// Defines
// SPI instructions
#define WRITE_INST 0x02
#define READ_INST 0x03
#define READSTATUS_INST 0xA0
#define RTS_INST 0x80
#define BITMOD_INST 0x05
#define RESET_INST 0xC0

// CAN controller register addresses
#define CANCTRL 0x0F			// Control
#define CANSTAT 0x0E			// Status
#define CANINTE 0x2B			// Interrupt enable
#define CANINTF 0x2C            // Interrupt flag
#define TXB0CTRL 0x30			// Transmit buffer 0
#define TXB1CTRL 0x40			// Transmit buffer 1
#define TXB2CTRL 0x50			// Transmit buffer 2
#define RXB0CTRL 0x60			// Receive buffer 0
#define RXB1CTRL 0x70			// Receive buffer 1

// CANCTRL: Control
#define MODE_MASK 0xE0			// For changing mode
#define MODE_NORMAL 0x00
#define MODE_SLEEP 0x20
#define MODE_LOOPBACK 0x40
#define MODE_LISTENONLY 0x60
#define MODE_CONFIG 0x80
#define MODE_POWERUP 0xE0

// CANINTE: Interrupt enable
#define CANINTE_MASK 0xFF
#define RX0IE 0x01				// Receive buffer 0 full
#define RX1IE 0x02				// Receive buffer 1 full
#define TX0IE 0x04				// Transmit buffer 0 empty
#define TX1IE 0x08				// Transmit buffer 1 empty
#define TX2IE 0x10				// Transmit buffer 2 empty

// CANINTF: Interrupt flag
#define CANINTF_MASK 0xFF
#define RX0IF 0x01				// Receive buffer 0 full
#define RX1IF 0x02				// Receive buffer 1 full
#define TX0IF 0x04				// Transmit buffer 0 empty
#define TX1IF 0x08				// Transmit buffer 1 empty
#define TX2IF 0x10				// Transmit buffer 2 empty

// RXBnCTRL: Receive control
#define RXM_MASK 0x60			// Mask for receive mask/filters
#define RXM_OFF 0x60			// Mask/filters off, receives any message
#define DLC_MASK 0x0F			// For reading data length

// TXB0CTRL: Transmit buffer 0
#define TXB0SIDH 0x31			// Standard identifier register high
#define TXB0SIDL 0x32			// Standard identifier register low
#define TXB0EIDH 0x33			// Standard identifier register high
#define TXB0EIDL 0x34			// Standard identifier register low
#define TXB0DLC 0x35			// Data length code register
#define TXB0D 0x36				// Data byte register (0x36-3D)

// TXB1CTRL: Transmit buffer 1
#define TXB1SIDH 0x41			// Standard identifier register high
#define TXB1SIDL 0x42			// Standard identifier register low
#define TXB1EIDH 0x43			// Standard identifier register high
#define TXB1EIDL 0x44			// Standard identifier register low
#define TXB1DLC 0x45			// Data length code register
#define TXB1D 0x46				// Data byte register (0x46-4D)

// TXB2CTRL: Transmit buffer 2
#define TXB2SIDH 0x51			// Standard identifier register high
#define TXB2SIDL 0x52			// Standard identifier register low
#define TXB2EIDH 0x53			// Standard identifier register high
#define TXB2EIDL 0x54			// Standard identifier register low
#define TXB2DLC 0x55			// Data length code register
#define TXB2D 0x56				// Data byte register (0x56-5D)

// RXB0CTRL: Receive buffer 0
#define RXB0SIDH 0x61			// Standard identifier register high
#define RXB0SIDL 0x62			// Standard identifier register low
#define RXB0EIDH 0x63			// Extended identifier register high
#define RXB0EIDL 0x64			// Extended identifier register low
#define RXB0DLC 0x65			// Data length code register
#define RXB0D 0x66				// Data byte register (0x66-6D)

// RXB0CTRL: Receive buffer 1
#define RXB1SIDH 0x71			// Standard identifier register high
#define RXB1SIDL 0x72			// Standard identifier register low
#define RXB1EIDH 0x73			// Extended identifier register high
#define RXB1EIDL 0x74			// Extended identifier register low
#define RXB1DLC 0x75			// Data length code register
#define RXB1D 0x76				// Data byte register (0x76-7D)

// readStatusCAN(): buffers
#define RBUFF_MASK 0x03
#define RBUFF0 0x01
#define RBUFF1 0x02
#define TBUFF_MASK 0x54
#define TBUFF0 0x01
#define TBUFF1 0x02
#define TBUFF2 0x04


// Functions
void writeCAN(uint8_t address, uint8_t data);
uint8_t readCAN(uint8_t address);
uint8_t readStatusCAN(void);
void sendRequestCAN(uint8_t buffers);
void modifyBitCAN(uint8_t address, uint8_t mask, uint8_t data);
void resetCAN(void);
uint8_t getModeCAN(void);
void setModeCAN(uint8_t mode);
uint8_t inModeCAN(uint8_t mode);


#endif /* CANCTRL_H_ */
