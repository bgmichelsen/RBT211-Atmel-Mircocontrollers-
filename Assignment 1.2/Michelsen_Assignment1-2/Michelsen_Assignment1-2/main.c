/*
 * Michelsen_Assignment1-2.c
 *
 * Created: 9/5/2018 2:03:21 PM
 * Author : brand
 */ 
#include <avr/io.h>
#include <util/delay.h>


int main(void)
{
	// Set PB3 and PB4 pins to outputs
	DDRB |= 0b00011000;
    // Run an infinite loop
    while (1) 
    {
		// Turn on PB3 and turn off PB4 for 1 second
		PORTB = 0b00001000;
		_delay_ms(1000);
		
		// Turn on PB4 and turn off PB3 for 1 second
		PORTB = 0b00010000;
		_delay_ms(1000);
    }
	
	return 0;
}

