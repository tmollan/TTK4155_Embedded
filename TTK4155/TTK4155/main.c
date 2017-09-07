/*
 * main.c
 *
 * Created: 07.09.2017 10:50:10
 *  Author: torkilsm
 */ 

#include <avr/io.h>
//#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>

#include "driverUART.h"


int main(void)
{
	// Init UART communication
	USART_Init(MYUBRR);
	
	
	stdout = &mystdout;
	
	printf("Working!");
	
    while(1)
    {
        //TODO:: Please write your application code 
		//PORTA = (1<<PA2);
		//_delay_ms(1000);
		//PORTA = (0<<PA2);
		//_delay_ms(1000);
		
		//unsigned char c = USART_Receive();
		//USART_Transmit(c);
		
    }
}
