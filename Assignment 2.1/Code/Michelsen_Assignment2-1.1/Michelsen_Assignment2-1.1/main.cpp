/*
 * Michelsen_Assignment2-1.1.cpp
 *
 * Created: 9/10/2018 10:42:53 AM
 * Author : Brandon Michelsen
 */ 

#include <avr/io.h>


int main(void)
{
    // Setup DDRB Pin 3 as an output
	DDRB |= (1<<DDB3);
	
	// Turn on the LED connected to PB3
	PORTB |= (1<<PB3);
	
	// Run infinitely
    while (1) 
    {
    }
	
	return 0;
}

