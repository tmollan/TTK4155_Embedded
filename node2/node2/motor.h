/*

motor.h

*/


#ifndef MOTOR_H_
#define MOTOR_H_

// Includes
#include "setup.h"
#include <stdint.h>
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "TWI.h"


// Defines
#define TWI_GEN_CALL 0x00  // The General Call address is 0

// Sample TWI transmission commands
#define TWI_CMD_WRITE 0x00
#define TWI_CMD_MASTER_READ 0x20

// Sample TWI transmission states, used in the main application.
#define SEND_DATA 0x01
#define REQUEST_DATA 0x02
#define READ_DATA_FROM_BUFFER 0x03

// TWI slave address
#define TWI_SLAVEADDR 0x50

// Encoder
#define DDRENCODER DDRH
#define DIRPIN PINH1
#define SELPIN PINH3
#define ENPIN PINH4
#define OEPIN PINH5
#define RSTPIN PINH6

// Solenoid
#define DDRSOL DDRB
#define PORTSOL PORTB
#define SOLPIN PINB7


// Functions
void initMotor(void);
void sendGeneralCallTWI(void);
void sendReadCommandTWI(uint8_t *msgBuffer);
uint8_t readTWI(void);
void writeTWI(uint8_t data);
int16_t readEncoder(void);
void runMotor(int8_t speed);
int8_t motorControl(int8_t ref);
void triggerSolenoid(void);


#endif /* MOTOR_H_ */