
// Includes
#include "motor.h"


void initMotor(void) {
	// Initializes Serial Interface TWI/I2C
	setBit(TWCR, TWEN);			// Enable 2-wire Serial Interface (SCL, SDA)
	setBit(PORTD, PIND0);		// Enable pull-up (SCL)
	setBit(PORTD, PIND1);		// Enable pull-up (SDA)
	
	TWI_Master_Initialise();
	sei();
	
	// Initializes motor interface
	makeInput(DDRENCODER, DIRPIN);
	makeInput(DDRENCODER, SELPIN);
	makeInput(DDRENCODER, ENPIN);
	makeInput(DDRENCODER, OEPIN);
	makeInput(DDRENCODER, RSTPIN);
	
	DDRK = 0x00;	// Make all K-pins inputs
	
	//setBit(DDRENCODER, ENPIN);		// Enable motor
	
	makeOutput(DDRSOL, SOLPIN);			// Enable solenoid output pin
}

// Send a General Call
void sendGeneralCallTWI(void) {
	uint8_t msgBuffer[4];
	msgBuffer[0] = TWI_GEN_CALL;     // The first byte must always consists of General Call code or the TWI slave address.
	msgBuffer[1] = 0xAA;             // The command or data to be included in the general call.
	TWI_Start_Transceiver_With_Data(msgBuffer, 2);
}

// Send the Address Call and request-for-data command to the Slave
void sendReadCommandTWI(uint8_t *msgBuffer) {
	// Address Call
	msgBuffer[0] = (TWI_SLAVEADDR << TWI_ADR_BITS) | (FALSE << TWI_READ_BIT);
	msgBuffer[1] = TWI_CMD_MASTER_READ;				// The first byte is used for commands.
	TWI_Start_Transceiver_With_Data(msgBuffer, 2);
	
	// Request data
	msgBuffer[0] = (TWI_SLAVEADDR << TWI_ADR_BITS) | (TRUE << TWI_READ_BIT);
	TWI_Start_Transceiver_With_Data(msgBuffer, 2);
}

// Reads data from TWI
uint8_t readTWI(void) {
	uint8_t msgBuffer[4];
	sendReadCommandTWI(msgBuffer);					// Sends Address Call and read request
	TWI_Get_Data_From_Transceiver(msgBuffer, 2);	// Reads data
	return msgBuffer[1];
}

// Send an Address Call, sending a command and data to the Slave
void writeTWI(uint8_t data) {
	uint8_t msgBuffer[4];
	msgBuffer[0] = TWI_SLAVEADDR;		
	msgBuffer[1] = TWI_CMD_WRITE;					// The first byte is used for commands.
	msgBuffer[2] = data;							// The second byte is used for the data.
	TWI_Start_Transceiver_With_Data(msgBuffer, 3);
}


int16_t readEncoder(void) {
	uint16_t data;
	
	clearBit(DDRENCODER, OEPIN);	// Set !OE low to enable output of encoder
	clearBit(DDRENCODER, SELPIN);	// Set SEL low to get high byte
	_delay_us(20);
	
	// Read MSB
	data = PINK;
	data = data << 8;
	
	setBit(DDRENCODER, SELPIN);		// Set SEL high to get low byte
	_delay_us(20);
	
	// Read LSB
	data |= PINK;
	
	//setBit(DDRENCODER, RSTPIN);		// Toggle !RST to reset encoder
	//clearBit(DDRENCODER, RSTPIN);
	
	setBit(DDRENCODER, OEPIN);		// Set !OE high to disable output of encoder
	
	return data;
}

void runMotor(int8_t speed) {
	uint8_t data;
	if (speed < 0) {
		setBit(DDRENCODER, DIRPIN);		// Set counter-clockwise rotation
		data = 0xFF*speed/100*-1;
	} else {
		clearBit(DDRENCODER, DIRPIN);	// Set clockwise rotation
		data = 0xFF*speed/100;
	}
	writeTWI(data);
}

int16_t intErr = 0;
int8_t motorControl(int8_t ref) {
	int16_t measurement = readEncoder();
	int16_t err = ref - measurement;
	
	intErr += err;
	if (intErr > 255) {
		intErr = 255;
	} else if (intErr < -255) {
		intErr = -255;
	}
	
	int8_t samplingTime = 1;
	int8_t integrationTime = 1;
	int8_t Pgain = 1;
	int8_t Igain = Pgain * samplingTime / integrationTime;
	
	//int8_t result = Pgain*err + Igain*intErr;
	int8_t result = Pgain*err;
	
	if (result > 100) {
		result = 100;
	} else if (result < -100) {
		result = -100;
	}
	
	return result;
}

void triggerSolenoid(void) {
	clearBit(PORTSOL, SOLPIN);
	_delay_ms(50);
	setBit(PORTSOL, SOLPIN);
	_delay_ms(50);
}