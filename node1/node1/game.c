
// Includes
#include "game.h"
#include "CAN.h"
#include "joystick.h"

void transmitGameInfo(gameFlags game) {
    joystick myJoystick = getJoystick();
	sliders mySliders = getSliders();
    int8_t joyButton = buttonPressed(JOYSTICK);
    int8_t leftButton = buttonPressed(LEFTBUTTON);
    int8_t rightButton = buttonPressed(RIGHTBUTTON);

    CANmessage msg;
    msg.id = 0x010F;		// Max 0x07ef
    msg.length = 7;
    msg.dataBytes[0] = game.byte;
    msg.dataBytes[1] = myJoystick.xPos;
    msg.dataBytes[2] = (uint8_t)(myJoystick.angle >> 8);
    msg.dataBytes[3] = (uint8_t)myJoystick.angle;
    msg.dataBytes[4] = myJoystick.dir;
    msg.dataBytes[5] = mySliders.leftPos;
    msg.dataBytes[6] = mySliders.rightPos;
    transmitCAN(msg);
}

gameFlags receiveGameInfo(void) {
    CANmessage msg = receiveCAN();
    if (msg.id == CANID_GAME) {
        return msg.dataBytes[0];
    }
    return 0
}
