/*
 * PWM.h
 *
 * Created: 30.10.2017 08:41:25
 *  Author: torkilsm
 */ 


#ifndef PWM_H_
#define PWM_H_

#include "setup.h"
#include <avr/io.h>
#include <stdint.h>

#define DDR_PWM DDRB
#define PWMPIN PINB5

void initPWN(void);
void driveServo(int8_t pos);


#endif /* PWM_H_ */