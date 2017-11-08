/*

PID.h

*/


#ifndef PID_H_
#define PID_H_

// Includes
#include <stdint.h>


// Defines
#define SCALING_FACTOR 128


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
void initPID(int16_t Kp, int16_t Ti, int16_t Td, int16_t T, PIDcontroller *PID);
void setModePID(controllerMode mode, PIDcontroller *PID);
void setParamPID(int16_t Kp, int16_t Ti, int16_t Td, int16_t T, PIDcontroller *PID);
int16_t PIDcontroller(int16_t setPoint, int16_t processValue, PIDcontroller *PID);


#endif /* PID_H_ */
