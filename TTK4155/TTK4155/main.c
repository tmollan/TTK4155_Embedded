
// Setup files and libraries
#include "setup.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>

// Header files
#include "driverUART.h"
#include "SRAM.h"

volatile char *OLED_COMMAND = (char*) 0x1000;
volatile char *ADC = (char*) 0x1400;
volatile char *SRAM = (char*) 0x1800;


int main(void) {
	// For printf() functionality
	stdout = &mystdout;

	// Initialize UART communication
	UART_Init(MYUBRR);
	printf("UART is up and running!\n");
	
	// Enables the external memory interface
	setBit(MCUCR, SRE);
	
	// Masks out PC4-PC7 for JTAG
	setBit(SFIOR, XMM2);
	
	//clearBit(PORTC, PC3);
	//setBit(PORTC, PC2);
	
	
	// Test program for SRAM IC
	//SRAM_test();
	
	while(1) {
		// Flashing LED
		//setBit(PORTA, PA0);
		//PORTA = 0x00;
		//_delay_ms(1000);
		//clearBit(PORTA, PA0);
		//PORTA = 0x01;
		//_delay_ms(1000);
		//_delay_ms(1);
		SRAM[6] = 0x0000;
		// UART comm. test: text entered in terminal should return
		//unsigned char c = UART_Receive();
		//UART_Transmit(c, '');
	}
}
