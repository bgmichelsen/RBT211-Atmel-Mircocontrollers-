/*
 * USART.cpp
 *
 * Created: 9/28/2018 2:30:32 PM
 *  Author: Brandon Michelsen
 */ 

#include "USART.h"
#include <avr/io.h>

// Method definitions for Serial class
void Serial::init(unsigned char baud_rate)
{
	// Set baud rate
	UBRR0H = 0;
	UBRR0L = baud_rate;
	
	// Set double clock speed to on
	UCSR0A |= (1<<U2X0);
	
	// Turn on receive and transmit
	UCSR0B |= ((1<<TXEN0) | (1<<RXEN0));
	
	// Use 8-bit mode
	UCSR0C |= ((1<<UCSZ00) | (1<<UCSZ01));
	
	return;
}

void Serial::transmit(unsigned char val)
{
	// Wait for transmit to be ready
	while ((UCSR0A & (1<<UDRE0)) == 0);
	
	// Write data out
	UDR0 = val;
	return;
}

char Serial::receive()
{
	// Wait for receive to be ready
	while ((UCSR0A & (1<<RXC0)) == 0);
	
	// Read in the value
	return UDR0;
}

void Serial::print(char* val)
{
	// Write the full string
	while(*val)
	{
		transmit(*val++);
	}
}