/*
 * Michelsen_Assignment6-1.4.cpp
 *
 * Created: 10/10/2018 5:23:38 PM
 * Author : brand
 */ 

#define F_CPU	16000000UL // 16 MHz clock

#include <avr/io.h>
#include <m328/serial/USART.h> // Library location for my Serial function

int main(void)
{
	// Setup inputs an outputs
	DDRB |= (1<<DDB0);
	
	// Initialize serial
	Serial serial_com;
	serial_com.init(BAUD9600);
	
	// Declare local variables
	char input; // Variable for reading data from the computer
	
    // Run infinitely
    while (1) 
    {
		// Read in a character from the computer
		input = serial_com.receive();
		
		// Use that toggle an LED on PB0
		if (input == '1')
		{
			PORTB |= (1<<PINB0);
			serial_com.print("LED ON\n");
		}
		else if (input == '0')
		{
			PORTB &= ~(1<<PINB0);
			serial_com.print("LED OFF\n");
		}
    }
}

