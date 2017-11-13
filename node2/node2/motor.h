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
#include "PID.h"


// Defines
#define TWI_GEN_CALL 0x00       // The General Call address is 0

// TWI transmission command
#define TWI_CMD_MASTER_WRITE 0x00

// TWI slave address
#define TWI_SLAVEADDR 0x50      // 0b0101000(0) where (0) is R/W bit

// Encoder pins
#define DDRENC DDRH
#define PORTENC PORTH
#define DIRPIN PINH1
#define SELPIN PINH3
#define ENPIN PINH4
#define OEPIN PINH5
#define RSTPIN PINH6
#define EDATA PINK

// Solenoid pin
#define DDRSOL DDRB
#define PORTSOL PORTB
#define SOLPIN PINB7


// Typedefs
typedef enum {
    MOTOR_CW,       // Clockwise
    MOTOR_CCW       // Counter-clockwise
} motorDirection;


// Functions
void initMotor(void);
void writeTWI(uint8_t data);
void setMotorDirection(motorDirection dir);
void runMotor(int8_t ref, PIDcontroller *PID);
int16_t readEncoder(void);
void resetEncoder(void);
int16_t calibrateEncoderMax(void);
void triggerSolenoid(void);


#endif /* MOTOR_H_ */
