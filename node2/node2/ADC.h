/*
 * IR.h
 *
 * Created: 30.10.2017 15:36:49
 *  Author: torkilsm
 */ 


#ifndef IR_H_
#define IR_H_

#include "setup.h"
#include <avr/io.h>
#include <stdint.h>


void initADC(void);
int8_t ballDetected(void);



#endif /* IR_H_ */