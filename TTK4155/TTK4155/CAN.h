/*

CAN.h

*/


#ifndef CAN_H_
#define CAN_H_

/*
#define LOOPBACK 0
typedef enum {
  LOOPBACK
} CANmode;
*/

typedef struct {
  uint8_t id;
  uint8_t data;
  uint8_t length;
} CANmessage;


void initCAN(void);
void writeCAN(CANmessage *data);
CANmessage readCAN(uint8_t buffer);


#endif /* CAN_H_ */
