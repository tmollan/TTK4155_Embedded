/*

setup.h

This file sets some important variables.
The file also contains some functions that might come in handy.

*/

#ifndef SETUP_H_
#define SETUP_H_


// Important variables
#define F_OSC 16000000          // Frequency of oscillator (clock speed) [Hz]
#define F_CPU F_OSC             // Frequency of CPU [Hz]
#define BAUD 9600               // Baud rate for communication
#define MYUBRR F_OSC/16/BAUD-1  // Baud rate register setting


// Functions that makes register manipulation easier
#define setBit(reg, bit) (reg |= (1 << bit))        // Set bit in reg. (rest as is)
#define clearBit(reg, bit) (reg &= ~(1 << bit))     // Clear bit in reg. (rest as is)
#define toggleBit(reg, bit) (reg ^= (1 << bit))     // Toggle bit in reg. (rest as is)
#define testBit(reg, bit) (reg & (1 << bit))        // 1 if set, 0 if clear
#define makeInput(reg, pin) clearBit(reg, pin)      // Sets pin to be input
#define makeOutput(reg, pin) setBit(reg, pin)       // Sets pin to be output
#define loopUntilSet(reg, bit) while(!testBit(reg, bit))    // Loops if bit is 0
#define loopUntilClear(reg, bit) while(testBit(reg, bit))   // Loops if bit is 1


#endif /* SETUP_H_ */
