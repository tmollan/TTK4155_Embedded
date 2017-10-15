/*

CANCTRL.h

*/


#ifndef CANCTRL_H_
#define CANCTRL_H_


void writeCAN(uint8_t data);
uint8_t readCAN(void);
uint8_t readStatusCAN(void);
// Should it return anything
uint8_t sendRequestCAN(void);
void modifyBitCAN(void);
void resetCAN(void);


#endif /* CANCTRL_H_ */
