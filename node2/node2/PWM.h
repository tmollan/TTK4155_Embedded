/*

PWM.h

*/

#ifndef PWM_H_
#define PWM_H_

// Includes
#include "setup.h"
#include <avr/io.h>
#include <stdint.h>

// Defines
#define DDR_PWM DDRB    // Servo pin
#define PWMPIN PINB5    // Servo pin

// PWM_TOP = F_OSC/F_PWM/PRESCALER-1 where F_PWM = 50Hz
#define PWM_TOP 39999


// Functions
void initPWM(void);
void driveServo(int8_t pos);


#endif /* PWM_H_ */
