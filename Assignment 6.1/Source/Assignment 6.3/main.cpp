/*
 * Michelsen_Assignment6-1.3.cpp
 *
 * Created: 10/10/2018 5:06:04 PM
 * Author : Brandon Michelsen
 */ 

#define F_CPU	16000000UL // 16 MHz clock

#include <avr/io.h>
#include "headers/USART.h"


int main(void)
{
	// Initialize serial
	Serial serial_com;
	serial_com.init(BAUD9600);
	
	// Declare local variables
	char input;
	
    // Run infinitely
    while (1) 
    {
		input = serial_com.receive();
		serial_com.transmit(input);	
		serial_com.transmit('\n');
    }
}

