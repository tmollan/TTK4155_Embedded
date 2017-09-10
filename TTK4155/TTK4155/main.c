
// Setup files and libraries
#include "setup.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>

// Header files
#include "driverUART.h"


int main(void) {
	// For printf() functionality
	stdout = &mystdout;

	// Initialize UART communication
	UART_Init(MYUBRR);
	printf("UART is up and running!");

    while(1) {

		/* // Flashing LED
		PORTA = (1<<PA2);
		_delay_ms(1000);
		PORTA = (0<<PA2);
		_delay_ms(1000);
		*/

		/* // UART comm. test: text entered in terminal should return
		unsigned char c = UART_Receive();
		UART_Transmit(c);
		*/

    }
}
