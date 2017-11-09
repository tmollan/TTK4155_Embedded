
// Includes
#include "PID.h"


void initPID(int16_t Kp, int16_t Ti, int16_t Td, int16_t Ts_ms, PIDcontroller *PID) {
    PID->mode = FEEDBACK;
    PID->sumError = 0;
    PID->prevProcessValue = 0;
    PID->pGain = SCALING_FACTOR * Kp;                       // Proportional gain
    PID->iGain = SCALING_FACTOR * Kp * Ts_ms/1000 / Ti;     // Integral gain
    PID->dGain = SCALING_FACTOR * Kp * Td / Ts_ms/1000;     // Derivative gain
    PID->maxError = INT16_MAX / (PID->pGain + 1);
    PID->maxSumError = INT32_MAX / 2 / (PID->iGain + 1);

    // Timer for PID sampling time
    // Set timer mode to CTC with OCRnA register defining compare value
	setBit(TCCR3B, WGM32);

	// Set clock prescaling to clk/64 (64: prescaler value)
	setBit(TCCR3B, CS30);
	setBit(TCCR3B, CS31);

    // Reset timer
    TCNT3 = 0;

	// Set compare value to trigger compare match flag
	OCR3A = F_OSC * Ts_ms/1000 / 64 - 1;
}


void setModePID(controllerMode mode, PIDcontroller *PID) {
    PID->mode = mode;
}


void setParamPID(int16_t Kp, int16_t Ti, int16_t Td, int16_t Ts_ms, PIDcontroller *PID) {
    PID->pGain = SCALING_FACTOR * Kp;                       // Proportional gain
    PID->iGain = SCALING_FACTOR * Kp * Ts_ms/1000 / Ti;     // Integral gain
    PID->dGain = SCALING_FACTOR * Kp * Td / Ts_ms/1000;     // Derivative gain
}


int16_t PIDcontrol(int16_t setPoint, int16_t processValue, PIDcontroller *PID) {
	int16_t error = setPoint - processValue;   // e(k) = r(k) - y(k)
	printf("process: %d\n", processValue);
	
	// Calculating proportional term and limiting overflow
    int16_t pTerm;
	if (error > PID->maxError) {
		pTerm = INT16_MAX;
	} else if (error < -PID->maxError) {
		pTerm = -INT16_MAX;
	} else {
		pTerm = PID->pGain * error;           // Kp * e(k)
	}

	// Calculating integral term with anti-windup implementation
    int32_t iTerm;
	int32_t tempError = PID->sumError + error;
	if (tempError > PID->maxSumError) {
		PID->sumError = PID->maxSumError;
		iTerm = INT32_MAX / 2;
	} else if (tempError < -PID->maxSumError) {
		PID->sumError = -PID->maxSumError;
		iTerm = -INT32_MAX / 2;
	} else {
		PID->sumError = tempError;
		iTerm = PID->iGain * PID->sumError;   // Ki * sum[e(0..k)]
	}

	// Calculating derivative term - Kd * (y(k) - y(k-1))
    int16_t dTerm = PID->dGain * (processValue - PID->prevProcessValue);
	PID->prevProcessValue = processValue;  // Updating previous value y(k-1)
	
	// Calculate and limit output
    int32_t output = (pTerm + iTerm + dTerm) / SCALING_FACTOR;
	if (output > INT16_MAX) {
		output = INT16_MAX;
	} else if (output < -INT16_MAX) {
		output = -INT16_MAX;
	}

	return (int16_t)output;
}
