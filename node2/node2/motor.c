
// Includes
#include "motor.h"

// Globals
int16_t encoderMax = 0;

// Initializes motor encoder
void initMotor(void) {
	// Initializes Serial Interface TWI/I2C
	setBit(TWCR, TWEN);			// Enable 2-wire Serial Interface (SCL, SDA)
	setBit(PORTD, PIND0);		// Enable pull-up (SCL)
	setBit(PORTD, PIND1);		// Enable pull-up (SDA)
	TWI_Master_Initialise();
	sei();

	// Initialize motor interface/encoder
	makeOutput(DDRENC, DIRPIN);		// Motor dir. (0 = clockwise, 1 = counter-clockwise)
	makeOutput(DDRENC, SELPIN);		// Select high/low byte (0 = high, 1 = low)
	makeOutput(DDRENC, ENPIN);		// Motor enable
	makeOutput(DDRENC, OEPIN);		// Enable encoder output
	makeOutput(DDRENC, RSTPIN);		// Reset encoder
	DDRK = 0x00;					// Make all K-pins inputs

	setBit(PORTENC, ENPIN);			// Enable motor
	resetEncoder();					// Resets and enables encoder

	encoderMax = calibrateEncoderMax();

	// Initialize solenoid
	makeOutput(DDRSOL, SOLPIN);		// Enable solenoid output pin
	setBit(PORTSOL, SOLPIN);		// Solenoid low
}


// Send write command and data to a slave address
void writeTWI(uint8_t data) {
	uint8_t msgBuffer[4];
	msgBuffer[0] = TWI_SLAVEADDR | (FALSE << TWI_READ_BIT); 	// TWI slave address
	msgBuffer[1] = TWI_CMD_MASTER_WRITE;		// The first byte is used for commands
	msgBuffer[2] = data;						// The second byte is used for the data
	TWI_Start_Transceiver_With_Data(msgBuffer, 3);
}


// Sets rotation direction of motor
void setMotorDirection(motorDirection dir) {
	switch (dir) {
		case MOTOR_CCW:
			clearBit(PORTENC, DIRPIN);	// Counter-clockwise
			break;
		default:
			setBit(PORTENC, DIRPIN);	// Clockwise
	}
}


// Runs motor with a given setpoint
void runMotor(int8_t ref, PIDcontroller *PID) {
	int16_t speed;
	switch (PID->mode) {
		case OPEN_LOOP:
			speed = 0xFF * ref/100;
			break;
		default:	// case FEEDBACK
			speed = PIDcontrol(encoderMax/2/100*ref, readEncoder(), PID);
	}

	if (speed < 0) {
		setMotorDirection(MOTOR_CCW);	// Set counter-clockwise direction
	} else {
		setMotorDirection(MOTOR_CW);	// Set clockwise direction
	}

	writeTWI((uint8_t)abs(speed));
}


// Reads rotation of motor axis
int16_t readEncoder(void) {
	clearBit(PORTENC, OEPIN);	// Set !OE low to enable output of encoder
	clearBit(PORTENC, SELPIN);	// Set SEL low to get high byte
	_delay_us(20);

	uint16_t data;
	data = EDATA << 8;			// Read MSB

	setBit(PORTENC, SELPIN);	// Set SEL high to get low byte
	_delay_us(20);

	data |= EDATA;				// Read LSB

	//resetEncoder();			// Toggle !RST to reset encoder counter
	setBit(PORTENC, OEPIN);		// Set !OE high to disable output of encoder

	return data;
}

// Resets encoder counter
void resetEncoder(void) {
	clearBit(PORTENC, RSTPIN);	// Reset encoder counter
	_delay_ms(20);
	setBit(PORTENC, RSTPIN);	// Enable encoder
	_delay_ms(20);
}

// Calibrates encoder with 0 at center of board
int16_t calibrateEncoderMax(void) {
	// Go to edge of rail
	setMotorDirection(MOTOR_CW);
	writeTWI(100);
	_delay_ms(1500);

	resetEncoder();

	// Go to other edge of rail
	setMotorDirection(MOTOR_CCW);
	writeTWI(100);
	_delay_ms(1500);

	// Save max value
	int16_t max = readEncoder();
	int16_t currentVal = max;

	// Go to center of rail
	setMotorDirection(MOTOR_CW);
	writeTWI(80);
	while (currentVal >= max/2) {
		currentVal = readEncoder();
	}
	writeTWI(0);	// Stop at center

	resetEncoder();
	return max;
}


// Triggers a solenoid pulse
void triggerSolenoid(void) {
	clearBit(PORTSOL, SOLPIN);
	_delay_ms(50);
	setBit(PORTSOL, SOLPIN);
	_delay_ms(50);
}
