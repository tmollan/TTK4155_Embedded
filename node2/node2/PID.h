/*

PID.h

*/


#ifndef PID_H_
#define PID_H_

// Includes
#include "setup.h"
#include <stdint.h>
#include <stdlib.h>
#include <avr/io.h>


// Defines
// PID parameters
#define P_GAIN 8		// Proportional gain
#define I_TIME 10		// Integral time 	[s]
#define D_TIME 1000		// Derivative time 	[s]
#define S_TIME 10		// Sample time 		[ms]

#define SCALING_FACTOR 128  // For more precise control


// Typedefs
typedef enum {
    OPEN_LOOP,
    FEEDBACK
} controllerMode;

typedef struct {
    controllerMode mode;
    int16_t pGain;
    int16_t iGain;
    int16_t dGain;
    int32_t sumError;
    int16_t maxError;
    int32_t maxSumError;
    int16_t prevProcessValue;
} PIDcontroller;


// Functions
void initPID(int16_t Kp, int16_t Ti, int16_t Td, int16_t Ts_ms, PIDcontroller *PID);
void setModePID(controllerMode mode, PIDcontroller *PID);
int16_t PIDcontrol(int16_t setPoint, int16_t processValue, PIDcontroller *PID);


#endif /* PID_H_ */
